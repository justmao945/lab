'use strict';

/*
window.onbeforeunload = function() {
    return "You're leaving the site.";
};
*/

// Declare app level module which depends on filters, and services
angular.module('myApp', [
  'ngRoute',
  'ui.bootstrap',
  'angular-md5',
  'luegg.directives',
  'myApp.filters',
  'myApp.services',
  'myApp.directives',
  'myApp.controllers'
]).
config(['$routeProvider', function($routeProvider) {
  $routeProvider.when('/chat', {templateUrl: 'partials/chat/main.html', controller: 'MyChatCtrl'});
  $routeProvider.otherwise({redirectTo: '/chat'});
}]);
