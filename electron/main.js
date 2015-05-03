var app = require("app");
var bw = require("browser-window");
require ('crash-reporter').start();

var mw = null;

app.on('window-all-closed', function() {
    if(process.platform != 'darwin') {
        app.quit();
    }
});

app.on('ready', function() {
    mw = new bw({width: 800, height: 600});
    mw.loadUrl('file://' + __dirname + '/index.html');
    mw.on('closed', function() {
        mw = null;
    });
});
