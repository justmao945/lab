'use strict';

filters.

filter('sortChatUsers', [function() {
    return function(users) {
        var sortable = [];
        for (var email in users) {
            sortable.push(users[email]);
        }
        sortable.sort(function(a, b) {
            return a.email.localeCompare(b.email);
        });
        return sortable;
    };
}])

.filter('sortChatUsersWithLastUpdate', ['chatData', function(chatData) {
    return function(users) {
        var sortable = [];
        for (var email in users) {
            sortable.push(users[email]);
        }
        sortable.sort(function(a, b) {
            var ma = chatData.messages[a.email],
                mb = chatData.messages[b.email];
            if(ma.last != mb.last) {
                return mb.last - ma.last;
            }else {
                return a.email.localeCompare(b.email);
            }
            return a.email.localeCompare(b.email);
        });
        return sortable;
    };
}]);