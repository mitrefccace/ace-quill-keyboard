const path = require('path');
var fs = require('fs');


const CSS = [
    'bootstrap/dist/css/bootstrap.min.css',
];

if (!fs.existsSync('./views/assets')) {
    fs.mkdirSync('./views/assets');
}

if (!fs.existsSync('./views/assets/css')) {
    fs.mkdirSync('./views/assets/css');
}

CSS.map(asset => {
    let filename = asset.substring(asset.lastIndexOf("/") + 1);
    let from = path.resolve(__dirname, `./node_modules/${asset}`)
    let to = path.resolve(__dirname, `./views/assets/css/${filename}`)
    if (fs.existsSync(from)) {
        fs.createReadStream(from).pipe(fs.createWriteStream(to));
    } else {
        console.log(`${from} does not exist.\nUpdate the build.js script with the correct file paths.`)
        process.exit(1)
    }
});

