const UserService = require('../services/UsersService');

const UserController = {
  addUser: async (req, res) => {
    let {username, password, age} = req.body;
    const data = await UserService.addUser(username, password, age)
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
    const data = await UserService.updateUser(userID, username, password, age)
    res.send({code:0, message:data})
  },

  login: async (req, res) => {
    let {username, password} = req.body;
    const data = await UserService.login(username, password)
    if(data.length == 0) {
      res.send({code:1, message:data})
    } else {
      // 登录成功，设置 cookie
      req.session.user = data[0] /// 挂载 session 对象, 存储于内存中
      res.send({code:0, message:data})
    }
  },

  logout: async (req, res) => {
    req.session.destroy(()=>{
      res.send({code:0, message:'退出成功'})
    })
  },
}

module.exports = UserController;
