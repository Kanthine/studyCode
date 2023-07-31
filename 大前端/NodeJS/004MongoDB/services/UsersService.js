const UserModel = require('../model/UserModel');

const UserService = {
  addUser:(username, password, age) => {
    return UserModel.create({username, password, age})
  },

  deleteUser:(userID) => {
    return UserModel.deleteOne({_id:userID})
  },

  getUsers:() => {
    return UserModel.find({}, ['username', 'age']).sort({age:1})
  },

  updateUser:(userID, username, password, age) => {
    return UserModel.updateOne({_id:userID}, {username, password, age})
  },

  login:(username, password) => {
    return UserModel.find({username, password})
  },
}

module.exports = UserService;
