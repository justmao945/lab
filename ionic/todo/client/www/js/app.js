

angular.module('todo', ['ionic'])

.controller('TodoCtrl', function($scope) {
    $scope.tasks = [
    	{ title: 'Why miss one ?' },
        { title: 'Conllect coins' },
        { title: 'Eat mushrooms' },
        { title: 'Get high enough to grab the flag' },
        { title: 'Find the Princess' }
    ];
});