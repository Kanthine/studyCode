const { update } = require('../model/UserModel');
const UserService = require('../services/UsersService');
const JWT = require('../unit/JWT');

const UserController = {
  addUser: async (req, res) => {
    const avatar = req.file ? `/uploads/${req.file.filename}` : `/uploads/default.png`
    console.log('add:', avatar, req.file)
    let {username, password, age} = req.body;
    const data = await UserService.addUser(username, password, age, avatar)
    res.send({code:0, message:data})
  },

  deleteUser: async (req, res) => {
    let {userID} = req.params;
    const data = await UserService.deleteUser(userID)
    res.send({code:0, message:data})
  },

  getUsers: async (req, res) => {
    const data = await UserService.getUsers()
    res.send({code:0, message:data})
  },

  updateUser: async (req, res) => {
    let {username, password, age} = req.body;
    let {userID} = req.params;
    const avatar = req.file ? `/uploads/${req.file.filename}` : `/uploads/default.png`
    const data = await UserService.updateUser(userID, username, password, age, avatar)
    res.send({code:0, message:data})
  },

  login: async (req, res) => {
    let {username, password} = req.body;
    const data = await UserService.login(username, password)
    if(data.length == 0) {
      res.send({code:1, message:data})
    } else {
      // 登录成功，存储 token
      var info = {_id: data[0]._id, username: data[0].username}
      const token = JWT.generate(info, '1d')
      res.header('Authorization', token)
      res.send({code:0, message:data})
    }
  },

  logout: async (req, res) => {
    res.send({code:0, message:'退出成功'})
  },
}

module.exports = UserController;
