//// User object
function User (socket, email, passwd) {
    this.email = email;
    this.passwd = passwd;
    this.socket = socket;
}

User.prototype.getPublicInfo = function() {
    return {
        email: this.email
    };
};


/// Users Object
function Users() {
    // { email: user }
    this.users = {};

    // {socketId: user}
    this.id2user = {};
    // contains unsigned users, guests
    // {socketId: socket}
    this.id2socket = {};
}

Users.prototype.hasUserMail = function(email) {
    return email in this.users;
};

Users.prototype.addUser = function(socket, email, passwd) {
    var user = new User(socket, email, passwd);
    this.users[email] = user;
    this.id2user[socket.id] = user;
    delete this.id2socket[socket.id];
};

Users.prototype.addGuest = function(socket) {
    assert(! (socket.id in this.id2user) );
    this.id2socket[socket.id] = socket;
};

Users.prototype.removeUserBySocket = function(id) {
    var user = this.id2user[id];
    if(user) {
        this.id2socket[id] = user.socket;
        delete this.users[user.email];
    }
    delete this.id2user[id];
};

Users.prototype.removeGuestBySocket = function(id) {
    delete this.id2socket[id];
};

Users.prototype.getUsersPubInfo = function() {
    var result = {};
    for(var email in this.users) {
        result[email] = this.users[email].getPublicInfo();
    }
    return result;
};

Users.prototype.traverseUsers = function(users, callback) {
    if(users === null) {
        for (var user in this.users) {
            callback(user, this.users[user]);
        }
    }
    else if(users.constructor === Array) {
        for (var i = 0; i < users.length; ++i) {
            callback(users[i], this.users[users[i]]);
        }
    }
};


/////
module.exports = {
  User:  User,
  Users: Users,
  def: {
      pub: 'pub@group.internal'
  }
};