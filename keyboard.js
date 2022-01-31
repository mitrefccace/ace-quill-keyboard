
const SerialPort = require('serialport'),
  utf8 = require('utf8'),
  KEY_DEL = Buffer.from([130]),
  KEY_BS = Buffer.from([131]),
  KEY_RIGHT = Buffer.from([132]),
  KEY_LEFT = Buffer.from([133]),
  KEY_UP = Buffer.from([134]),
  KEY_DOWN = Buffer.from([135]),
  KEY_PGUP = Buffer.from([136]),
  KEY_PGDOWN = Buffer.from([137]),
  KEY_ENTER = Buffer.from([138]),
  KEY_INS = Buffer.from([140]),
  KEY_ESC = Buffer.from([139]);
KEY_TAB = Buffer.from([141]);

const { promisify } = require('util');
const sleep = promisify(setTimeout);

const InterByteTimeout = require('@serialport/parser-inter-byte-timeout')




function Keyboard(serialPort, baudRate = 57600) {
  console.log("Opening new keyboard connection")
  this.port = new SerialPort(serialPort, {
    baudRate: baudRate
  })
  this.isOpen = true;

  this.port.pipe(new InterByteTimeout({ interval: 30 }))
};

Keyboard.prototype.type = async function (sentence, pacing = 200) {
  try {
    var textArr = sentence.split('');
    for (let i = 0; i < textArr.length; i++) {
      await sleep(pacing)
      let letter = textArr[i];
      if (letter == "|") {
        i += 1;
        command = textArr[i];
        if (command == "I") {
          i += 1;
          switch (textArr[i]) {
            case 'x':
              command = KEY_DEL;
              break;
            case 'b':
              command = KEY_BS;
              break;
            case 'l':
              command = KEY_RIGHT;
              break;
            case 'h':
              command = KEY_LEFT;
              break;
            case 'k':
              command = KEY_UP;
              break;
            case 'j':
              command = KEY_DOWN;
              break;
            case 'u':
              command = KEY_PGUP;
              break;
            case 'd':
              command = KEY_PGDOWN;
              break;
            case 'n':
              command = KEY_ENTER;
              break;
            case 'i':
              command = KEY_INS;
              break;
            case 'e':
              command = KEY_ESC;
              break;
            case 't':
              command = KEY_TAB;
              break;
          }
          this.port.write(command)
        } else if (command == "P") {
          pacing = Number(sentence.substring(i + 1, i + 5))
          i += 4;
        } else {
          this.port.write(utf8.encode(command));
        }
      } else {
        this.port.write(utf8.encode(letter))
      }
    }

  } catch (err) {
    console.log("Keyboard Error:", err);
  }
};

Keyboard.prototype.close = async function () {
  if (this.isOpen) {
    this.port.close(function (err) {
      console.log("port closed", err)
      this.isOpen = false;
    });
  }
};

module.exports = Keyboard;









