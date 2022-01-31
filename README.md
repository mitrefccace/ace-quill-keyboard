## ACE Quill Keyboard
The Electron ACE Quill Keyboard app interfaces the ACE Quill terminal with a Teensy 4.1 microcontroller. 

### Installation
- `cd ace-quill-keyboard`
- `npm install`
- Plug Teensy Keyboard into into your computer
- `npm start` 

### Building the Standalone App
- `cd ace-quill-keyboard`
- Build for MacOS:
    - `npm run build-darwin`
- Build for Windows:
    - `npm run build-win32`
- Build for Linux:
    - `npm run build-linux`
- Build for all platforms:
    - `npm run build-all`
- Application will be in `ace-quill-keyboard/electron/dist`

