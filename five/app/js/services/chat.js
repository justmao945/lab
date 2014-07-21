'use strict';

services.

value('chatData', {
    def: {
       pub: 'pub@group.internal'
    },
    user: {
        email: '',
        passwd: '',
        status: {
            signed: false,
            exist: false
        }
    },
    target: {
        // user email is str@str.str
        // special emails: str@str.internal
        email: 'pub@group.internal'
    },
    // {
    //    email: user pub info,...
    // }
    users: {},
    //
    recentUsers: {},
    //{
    //  email: {
    //      greeting: msg
    //      unread: 0
    //      last: Date
    //      current: {
    //          say: msg
    //      }
    //      history: [{
    //          from: email
    //          to: email
    //          when: date
    //          say: string
    //          status: {
    //              received: null/true/false
    //          }
    //      },...
    //      ],...
    //  }
    //}
    messages: {
        'pub@group.internal': {
            greeting: 'Hi, you can chat with others now.',
            unread: 0,
            last: 0,
            current: {
                say: null
            },
            history: []
        }
    }
}).

value('chatAlert', {
    type: null, // danger, warning, success, info...
    msg: null
}).

factory('chatUserTabStatus', [function() {
    var my = {
        activeTab1: true,
        activeTab2: false,
        activeTab3: false
    };
    my.activeTab = function (num) {
        my.activeTab1 = num === 1;
        my.activeTab2 = num === 2;
        my.activeTab3 = num === 3;
    };
    return my;
}]).

factory('chatSocket', ['socket', function(socket) {
    return socket.connect('/chat/socket');
}]).

factory('chatSrv', ['$rootScope', 'chatData', 'chatSocket', 'chatAlert',
        function($rootScope, chatData, chatSocket, chatAlert) {
    var io = chatSocket;
    var my = chatData;

    //
    io.on('disconnect', function() {
    });

    // auto login when reconnect
    io.on('connect', function() {
        if(my.user.email) {
            srv.signIn();
        }
    });

    // prepare events
    io.on('update:users', function(data) {
        my.users = data.users;
        // update empty data
        for(var user in my.users) {
            if(! (user in my.messages)) {
                my.messages[user] = {
                    unread: 0,
                    last: 0,
                    current: {
                        say: null
                    },
                    history: []
                };
            }
        }
        // update recent users
        for(var user in my.recentUsers) {
            if(! (user in my.users)) {
                delete my.recentUsers[user];
            }
        }
    });

    io.on('update:msg', function(data, confirm) {
        var msg, target;
        if(data.to !== my.user.email) {
            // public talk, group talk
            target = data.to;
        }
        else {
            // user to user
            confirm({received: true});
            target = data.from;
            my.recentUsers[target] = my.users[target];
        }
        msg = my.messages[target];
        msg.last = new Date();
        msg.history.push(data);

        if (my.target.email !== target) {
            // inactive users
            // 100 -> 99+
            if (msg.unread <= 99) {
                msg.unread += 1;
            }
        }
        $rootScope.$broadcast('new:message');
    });

    io.on('sign:out', function(status, confirm) {
        my.user = {
            email: '',
            passwd: '',
            status: status
        };
        confirm(status);
    });

     // service functions
    var srv = {
        signIn: function () {
            io.emit('sign:in', my.user, function (status) {
                my.user.status = status;
            });
        },
        signOut: function () {
            io.emit('sign:out', null, function (status) {
                my.user = {
                    email: '',
                    passwd: '',
                    status: status
                };
            });
        },
        sendMsg: function () {
            var msg = my.messages[my.target.email];
            var data = {
                send: true,
                from: my.user.email,
                to:   my.target.email,
                say:  msg.current.say,
                when: new Date(),
                status: {
                    received: null
                }
            };
            msg.history.push(data);
            if(data.to !== my.def.pub) {
                my.recentUsers[data.to] = my.users[data.to];
            }
            io.emit('send:msg', data, function (status) {
                msg.history[msg.history.length - 1].status = status;
                msg.current.say = '';
                msg.last = new Date();
            });
        },
        activeTarget: function (email) {
            my.target = {
                email: email
            };
            var msg = my.messages[email];
            msg.unread = 0;
        }
    };
    return srv;
}]);
