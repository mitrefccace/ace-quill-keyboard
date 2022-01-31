
// Parameters you may want to change 
  #define keyboardSerial Serial7 // Really, Keyboard input - Keyboard output is main USB
  #define keyboardSerialSpeed 57600

  #define debugSerial Serial // Troubleshooting output.
  #define debugSerialSpeed 57600

  String VERSION="1.0.0";

  bool          blinkerDebug   = true;       // Watchdog log to serial port
  int           blinkerPeriod  = 5 * 1000;   // milliseconds

  bool     OLEDPresent=true;  // Set to false if no display
  #define SCREEN_WIDTH   128  // OLED display width, in pixels
  #define SCREEN_HEIGHT  32   // OLED display height, in pixels
  #define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C // Usually 0x3D for 128x64, 0x3C for 128x32
  #define CHYRONLENGTH   10   // Number of keystrokes to display on OLED
  String  chyron="          ";// 5 blank spaces to start
  


//-----------------------------------------------------------------
// I2C OLED print

  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  #include <SPI.h>
  #include <Wire.h>
  extern TwoWire Wire2;// use SCL2 & SDA2

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire2, OLED_RESET);

void OLEDDisplayLine(byte textSize, byte row, String textToPrint, byte startChar=0, byte numChar=20) {
  // Overwrites and rewrites a given portion of the screen.
  // start and end are optional.  If not specified, will overwrite the entire line

  display.setTextSize(textSize);
  
  // No error checking for now - be careful!

  // Need character size in pixels to clear rectangle and
  // position cursor for writing
  byte charX=6*textSize;
  byte charY=8*textSize;
  
  display.setTextSize(textSize);
  // First, clear the area
  display.fillRect(startChar*charX, row*charY, numChar*charX, charY, SSD1306_BLACK);
  
  // Then, print the new string
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor( startChar*charX, row*charY); 
  display.print(textToPrint);
  display.display();
}

//-----------------------------------------------------------------
// debug print
//
// Log to Serial port and, if present, display 
// format "sss.d: message"
// Timestamp in seconds rolls every 100 seconds.
// ASSUMES 128x32 display!

void log(String message) {
  unsigned int currentTime=millis();
  String ts=String((currentTime/1000)%100)+"."+String((currentTime/100)%10); //seconds and tenths
  if (ts.length() < 4) {
    ts=" "+ts;  // pad it if necessary
  }
  debugSerial.println(ts+": "+message);

  if (OLEDPresent) {
    OLEDDisplayLine(1,3,message,0,20); // since it's the last line, extra characters are truncated
  }

}
//-----------------------------------------------------------------
// Keyboard Repeater

  byte keyboardReadByte; //inbound char to repeat
  
  // "Special" key definitions - unique mapping, needs to match sender
  // These need to be updated - I missed some
  # define keyDel    130
  # define keyBS     131
  # define keyRight  132
  # define keyLeft   133
  # define keyUp     134
  # define keyDown   135
  # define keyPGUp   136
  # define keyPGDown 137
  # define keyEnter  138
  # define keyESC    139
  # define keyINS    140
  # define keyTab    141

void checkForKeySent() {
  byte readByte;
  if (keyboardSerial.available()) {
    readByte = keyboardSerial.read();
    log(char(readByte));
      
    if (readByte == keyDel){
      usb_keyboard_press(KEY_DELETE, 0);
      chyron=chyron.substring(5,CHYRONLENGTH-1)+"<del>";
    }
    else if (readByte == keyBS){
      usb_keyboard_press(KEY_BACKSPACE, 0);
      chyron=chyron.substring(4,CHYRONLENGTH-1)+"<bs>";
    }
    else if (readByte == keyRight){
      usb_keyboard_press(KEY_RIGHT,0);
      chyron=chyron.substring(6,CHYRONLENGTH-1)+"<rght>";
    }
    else if (readByte == keyLeft){
      usb_keyboard_press(KEY_LEFT, 0);
      chyron=chyron.substring(6,CHYRONLENGTH-1)+"<left>";
    }
    else if (readByte == keyUp){
      usb_keyboard_press(KEY_UP, 0);
      chyron=chyron.substring(4,CHYRONLENGTH-1)+"<up>";
    }
    else if (readByte == keyDown){
      usb_keyboard_press(KEY_DOWN, 0);
      chyron=chyron.substring(5,CHYRONLENGTH-1)+"<dwn>";
    }
     else if (readByte == keyPGUp){
      usb_keyboard_press(KEY_PAGE_UP, 0);
      chyron=chyron.substring(6,CHYRONLENGTH-1)+"<pgup>";
    }
    else if (readByte == keyPGDown){
      usb_keyboard_press(KEY_PAGE_DOWN, 0);
      chyron=chyron.substring(6,CHYRONLENGTH-1)+"<pgdn>";
    }
     else if (readByte == keyEnter){
      usb_keyboard_press(KEY_ENTER, 0);
      chyron=chyron.substring(5,CHYRONLENGTH-1)+"<ent>";
    }
     else if (readByte == keyINS){
      usb_keyboard_press(KEY_INSERT, 0);
      chyron=chyron.substring(5,CHYRONLENGTH-1)+"<ins>";
    }
     else if (readByte == keyTab){
      usb_keyboard_press(KEY_TAB, 0);
      chyron=chyron.substring(5,CHYRONLENGTH-1)+"<tab>";
    }
    else {
      log(char(readByte));
      Keyboard.write(char(readByte));
      chyron=chyron.substring(1,CHYRONLENGTH-1)+char(readByte);
    }
    OLEDDisplayLine(2, 0, chyron); 
  } else {
    //log("no keyboard");
  }
}

//-----------------------------------------------------------------
// non-blocking blinker
// 
  byte          blinkerState   = HIGH;       // start lit up
  unsigned long blinkerLast    = 0;          // tracks time of last state change

void blinkSetup (){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, blinkerState);   // Light it up
}

void blinkLoop(int period) {
  if (millis() > blinkerLast+period) { //doesn't handle rollover - will fail after ~50 days
    if (blinkerState==LOW) {
      blinkerState=HIGH;
      log("ACE Quill Keyboard.");
      }
    else {
      blinkerState=LOW;
      log("ACE Quill Keyboard");
      }
    
    blinkerLast=blinkerLast+period;
    
    digitalWrite(LED_BUILTIN, blinkerState);
  }
}


//-----------------------------------------------------------------

#include "Keyboard.h"
void setup() {
  if (OLEDPresent) {
    // Set up OLED

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      OLEDPresent=false;  
    }
  
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    OLEDDisplayLine(4,0,"MITRE");
  }
  
  // Wait a bit for USB Enumeration, then clear MITRE LOGO
  delay(3 * 1000);
  if (OLEDPresent) {
    display.clearDisplay();
    display.display();
  }
  
  // Set up debug and keyboard input Serial port
  keyboardSerial.begin(keyboardSerialSpeed);
  debugSerial.begin(debugSerialSpeed);
  log("version " + VERSION);
  
  // flash the LED so we know something is happening
  blinkSetup();
  Keyboard.begin();
}

void loop() {
  blinkLoop(blinkerPeriod);
  checkForKeySent();
}
