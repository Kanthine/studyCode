const express = require('express')
const app = express()
const homeRouter = require('./router/homeRouter')
const listRouter = require('./router/listRouter')


app.listen(3100, ()=>{
    console.log('app start')
})

// ********************************* 中间件 *********************************
// Express 是一个自身功能极简，完全是由路由和中间件构成一个的 web 开发框架:从本质上来说，一个 Express 应用就是在调用各种中间件。
// 
// 中间件(Middleware) 是一个函数，它可以访问请求对象(request object (req)), 响应对象 (response object (res)), 
// 和 web 应用中处于请求-响应循环流程中的中间件，一般被命名为 next 的 变量。

app.get('/login', (req, res)=>{
    res.tokenInfo = '123456'
    // localStorage.setItem('tokenInfo', res.tokenInfo);
    res.send('login 成功!');
})

const verifyToken=(req, res, next)=>{
    // const token = window.localStorage.getItem('tokenInfo')
    let token = '123456'
    // token = null
    if (!token) {
        res.send('verifyToken 失败!');
    } else {
        res.tokenInfo = token
        console.log(`verifyToken success: ${token}!`);
        next()
    }
}

// 应用级中间件: 挂在 App 上
app.use(verifyToken) 
app.use('/home', homeRouter)
app.use('/list', listRouter)

// 错误中间件
app.use( (err, req, res, next)=>{
    console.error('错误中间件：' ,err.stack)
    res.status(500).send('Something broke!')
})
