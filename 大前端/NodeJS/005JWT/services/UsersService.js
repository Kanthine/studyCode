const UserModel = require('../model/UserModel');

const UserService = {
  addUser:(username, password, age, avatar) => {
    return UserModel.create({username, password, age, avatar})
  },

  deleteUser:(userID) => {
    return UserModel.deleteOne({_id:userID})
  },

  getUsers:() => {
    return UserModel.find({}, ['username', 'age', 'avatar']).sort({age:1})
  },

  updateUser:(userID, username, password, age, avatar) => {
    return UserModel.updateOne({_id:userID}, {username, password, age, avatar})
  },

  login:(username, password) => {
    return UserModel.find({username, password})
  },
}

module.exports = UserService;
