const mainWindow = require('../main.js').mainWindow;
const Keyboard = require('../keyboard.js');
const SerialPort = require('serialport');
var ipc = require('electron').ipcMain;
require('electron').ipcMain.setMaxListeners(0);
const Store = require('electron-store');
const store = new Store();


var express = require('express');
var router = express.Router();

var serialPortsAvailable = 0;
var serialPorts = [];

var keyboard;

/* GET home page. */
router.get('/', function (req, res, next) {
  res.send('Successfully Connected to ACE Quill Keyboard');
});

ipc.on('getSerialPorts', async function (event, args) {
  serialPortsAvailable = 0;
  sendSerialPorts();
});

ipc.on('regSP', async function (event, args) {
  if(typeof keyboard == 'object'){
    console.log("try to close keyboard")
    await keyboard.close();
  }
  if(store.get('serialPort'))
    keyboard = new Keyboard(store.get('serialPort'));
});

ipc.on('keyboardInput', async function (event, args) {
  try {

    if(keyboard)
      keyboard.type(args.sentence, args.pacing);
  } catch (err) {
    console.log("keyboardInput Error:",err);
  }

});

router.post('/type', function (req, res, next) {
  let p = store.get('pacing');
  let msg = req.body.message
  let pacing = req.body.pacing || p;
  try {
    keyboard.type(msg,  pacing);
  } catch (err) {
    console.log("/type Error:",err);
  }
  res.status(200).send()
});


setInterval(checkTimeout, 2000);

function checkTimeout() {
  if (mainWindow !== null) {
    sendSerialPorts();
  }
}

function sendSerialPorts() {
  SerialPort.list().then(function (data) {
    if (data.length != serialPortsAvailable) {
      serialPortsAvailable = data.length;
      serialPorts = []
      data.forEach(x => { serialPorts.push(x.path) });
      mainWindow.webContents.send('serialUpdate', { serialPorts });
    }
  });
}



module.exports = router;