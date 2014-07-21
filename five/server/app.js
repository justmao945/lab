// Setup basic express server
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var util = require('util')
var port = process.env.PORT || 8000;

var chat_handler = require('./ctrls/chat.js')

server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

// Routing
app.use(express.static(__dirname + '/../app'));

var chat = io
    .of('/chat/socket')
    .on('connection', function (socket) {
        util.debug('/chat/socket connected');
        chat_handler(socket);
    });

