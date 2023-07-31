var express = require('express');
const UserController = require('../controllers/UsersController');
var router = express.Router();

/// 增
router.post('', UserController.addUser)

// 删
router.delete('/:userID', UserController.deleteUser)

// 查
router.get('/', UserController.getUsers);

// 改
router.put('/:userID', UserController.updateUser)

// 登录校验
router.post('/login', UserController.login)

// 退出登录
router.get('/logout', UserController.logout)

module.exports = router;

/// npm install mongoose