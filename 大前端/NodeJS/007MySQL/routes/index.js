var express = require('express');
const JWT = require('../unit/JWT');
var router = express.Router();

router.get('/', function(req, res, next) {
  res.render('index', { title: 'MongDB 数据库' });
});

var token = JWT.generate({ data:'后端数据'}, '5s');
console.log('token ======= ', token)

var decoded = JWT.verify(token)
console.log('decoded ======= ', decoded)

setTimeout(() => { /// 过期
  var decoded = JWT.verify(token)
  console.log('decoded ======= ', decoded)
}, 10 * 1000);



module.exports = router;

/* 下载 SDK npm install express-session
 *
 * 1、cookie 是前端存储的一个信息，单独的 cookie 策略、前端数据可以伪造来欺骗后端，因此单纯的 cookie 策略不安全
 * 2、session 存储于后端，前端发送到后端的登录信息校验成功后、后端生成并存储一份 session、并将 sessionID 发送给前端用作前端的 cookie
 *    前端每次请求后端，都带着 sessionID 去请求，后端校验 sessionID 
 * 3、express-session 缺点：当用户量很大，后端数据库维护千万条 cookie 时，这对于服务器是一个巨大的开销；
 *                         如果前端的多个接口、对接多个后端服务器、如何让用户无感的共享一条 cookie ？
 *                         容易被跨站请求伪造
 * 
 * 
 * npm install jsonwebtoken
 */