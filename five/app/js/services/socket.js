'use strict';

// http://briantford.com/blog/angular-socket-io
services.
factory('socket', ['$rootScope', function($rootScope) {
    return {
        connect: function(addr) {
            var socket = io.connect(addr);
            return {
                on: function (eventName, callback) {
                    socket.on(eventName, function () {
                        var args = arguments;
                        $rootScope.$apply(function () {
                            callback.apply(socket, args);
                        });
                    });
                },
                emit: function (eventName, data, callback) {
                    socket.emit(eventName, data, function () {
                        var args = arguments;
                        $rootScope.$apply(function () {
                            if (callback) {
                                callback.apply(socket, args);
                            }
                        });
                    });
                }
            };
        }
    };
}]);
