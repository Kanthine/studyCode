const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    // 渲染模版后返回给前端
    res.render('login', {title:'登录页面', error:''}) /// 去 views 文件夹下寻找
})

router.post('/', (req, res)=>{
    let {username, password} = req.body;
    if (username && password) {
        /// 登录成功，重定向到 home 页面（由后端引导重定向）
        res.redirect('/home')
    } else {
        /// 登录失败，刷新 login 页面
        res.render('login',{title:'登录错误', error:'用户名或密码错误！'})
    }
})

module.exports= router;