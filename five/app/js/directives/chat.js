'use strict';

directives.
directive('myChatNav', function() {
    return {
        restrict: 'E',
        templateUrl: '../../partials/chat/nav.html'
    };
}).
directive('myChatUser', function() {
    return {
        restrict: 'E',
        templateUrl: '../../partials/chat/user.html'
    };
}).
directive('myChatWindow', function() {
    return {
        restrict: 'E',
        templateUrl: '../../partials/chat/window.html'
    };
});