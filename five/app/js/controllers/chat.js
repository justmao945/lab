'use strict';

controllers.

controller('MyChatCtrl', ['$window', '$scope', '$rootScope', 'chatSrv', 'chatData', 'chatUserTabStatus',
function($window, $scope, $rootScope, chatSrv, chatData, chatUserTabStatus) {
    $scope.my = {
        dat: chatData,
        srv: chatSrv,
        utab: chatUserTabStatus
    };

    var title = "Let's Chat";
    $rootScope.title = title;

    var isActiveWindow = true;
    $window.onfocus = function () {
        isActiveWindow = true;
        $scope.$apply(function() {
            $rootScope.title = title;
        });
    };

    $window.onblur = function () {
        isActiveWindow = false;
    };

    var unread = 0;
    $scope.$on('new:message', function(){
        if(isActiveWindow) {
            return;
        }
        unread += 1;
        if(unread > 0 && unread <= 99) {
            $rootScope.title = title + " (" + unread + ")";
        }
        else if(unread > 99) {
            $rootScope.title = title + " (99+)";
        }
    });
}])

.controller('MyChatAlertCtrl', ['$scope', 'chatAlert', function($scope, chatAlert){
    $scope.my = {
        alert: chatAlert,
        close: function() {
            chatAlert.type = null;
        }
    };
}]);