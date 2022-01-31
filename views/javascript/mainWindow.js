var ipc = require('electron').ipcRenderer;
require('electron').ipcRenderer.setMaxListeners(0);
const Store = require('electron-store');
const store = new Store();

window.onload = function () {
    console.log("get serialports")
    ipc.send('getSerialPorts');
};

const testbutton = document.getElementById('testbutton');
const savebutton = document.getElementById('savebutton');
var serialPorts = document.getElementById('serial');
var pacing = document.getElementById('pacing');
var testSentence = document.getElementById('testSentence')
var testkeyboard = document.getElementById('testkeyboard');

serialPorts.onchange = async function (evt) {
    console.log(">",serialPorts.value)
    store.set('serialPort', serialPorts.value);
    ipc.send('regSP');
}

testbutton.onclick = async function () {
    testkeyboard.value = ""
    testkeyboard.value = ""
    testkeyboard.focus();
    ipc.send('keyboardInput', {
        "serial": serialPorts.value,
        "sentence": testSentence.value,
        "pacing": pacing.value
    });
}

savebutton.onclick = async function () {
    console.log("aa")

    store.set('serialPort', serialPorts.value);
    store.set('pacing', pacing.value);


}
if (store.get('pacing')) {
    pacing.value = store.get('pacing')
}

if (store.get('serialPort')) {
    ipc.send('regSP');
}

ipc.on('serialUpdate', async function (event, data) {
    console.log("serialUpdate")
    serialPorts.innerHTML = "";
    for (i in data.serialPorts) {
        var opt = document.createElement("option");
        opt.value = data.serialPorts[i];
        opt.innerHTML = data.serialPorts[i];
        serialPorts.appendChild(opt);
    }
    var evt = document.createEvent("HTMLEvents");
    evt.initEvent("change", false, true);
    serialPorts.value = store.get('serialPort')

    serialPorts.dispatchEvent(evt);
});

