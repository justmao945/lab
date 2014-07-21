'use strict';

/* Filters */

var filters = angular.module('myApp.filters', []);

filters.

filter('interpolate', ['version', function(version) {
    return function(text) {
        return String(text).replace(/\%VERSION\%/mg, version);
    };
}]).

filter('gravatar', ['md5', function(md5) {
    var cache = {};
    return function(email) {
        if(! (email in cache) ) {
            cache[email] = "http://www.gravatar.com/avatar/" + md5.createHash(String(email).trim() || '');
        }
        return cache[email];
    }
}]);

