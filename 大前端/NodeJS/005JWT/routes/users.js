var express = require('express');
const UserController = require('../controllers/UsersController');
var router = express.Router();

// 存储头像文件至 public/uploads 文件夹下
const multer = require('multer');
const upload = multer({dest:'public/uploads/'})

/// 增
router.post('', upload.single('avatar'), UserController.addUser)

// 删
router.delete('/:userID', UserController.deleteUser)

// 查
router.get('/', UserController.getUsers);

// 改
router.put('/:userID', upload.single('avatar'), UserController.updateUser)

// 登录校验
router.post('/login', UserController.login)

// 退出登录
router.get('/logout', UserController.logout)

module.exports = router;

/// npm install mongoose