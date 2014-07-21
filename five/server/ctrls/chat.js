// Main handler for chat
var util = require('util');
var user = require('../models/user.js');

/////// Global vars
var users = new user.Users();


//////// main module
module.exports = function(socket) {
    // broadcast update
    // Send: {
    //      users: object
    //  }
    var updateUsers = function() {
        util.debug('broadcast update users');
        var data = {users: users.getUsersPubInfo() };
        socket.broadcast.emit('update:users', data);
        socket.emit('update:users', data);
    };

    // update when connected
    updateUsers();

    // Get:
    //  data: {
    //          email: str,
    //          passwd: str,
    //          force: bool
    //        }
    //  confirm: callback
    // Send:
    //  status: {
    //             exist:  bool
    //             signed: bool
    //          }
    socket.on('sign:in', function (data, confirm) {
        util.debug('chat on sign:in ' + data.email);
        var status = {
            exist: users.hasUserMail(data.email)
        };
        var signIn = function(){
            users.addUser(socket, data.email, data.passwd);
            updateUsers();
            status.signed = true;
            confirm(status);
        };

        users.traverseUsers([data.email], function(email, user){
            var status = {
                signed: false
            };
            if(user) {
                user.socket.emit('sign:out', status, function (status) {
                    users.removeUserBySocket(user.socket.id);
                    signIn();
                });
            }else {
                signIn();
            }
        });
    });

    // Get:
    //  data: null
    // Send:
    //  status: {
    //      signed: bool
    // }
    socket.on('sign:out', function (data, confirm) {
        util.debug('chat on sign:out ' + socket.id);
        var status = {};
        users.removeUserBySocket(socket.id);
        updateUsers();
        status.signed = false;
        confirm(status);
    });

    // Get:
    //  data: {
    //      send: true
    //      from: email
    //      to: email
    //      when: date
    //      say: msg
    //  }
    //  send: {
    //      send: false
    //      from: email
    //      to: email
    //      when: date
    //      say: msg
    //  }
    //
    // Get and send confirm:
    // {
    //      received: bool
    // }
    socket.on('send:msg', function(data, confirm){
        util.debug("chat on send:msg from " + data.from + " to " + data.to);
        data.send = false;
        var status = {
            received: true
        };
        if(data.to === user.def.pub) {
            socket.broadcast.emit("update:msg", data);
            confirm(status);
        }
        else {
            users.traverseUsers([data.to], function (email, user) {
                if (!user) {
                    status.received = false;
                    confirm(status);
                }
                else {
                    user.socket.emit('update:msg', data, function (status) {
                        confirm(status);
                    });
                }
            });
        }
    });

    socket.on('disconnect', function() {
        util.debug('chat on disconnected: ' + socket.id);
        users.removeUserBySocket(socket.id);
        users.removeGuestBySocket(socket.id);
        updateUsers();
    });
};

