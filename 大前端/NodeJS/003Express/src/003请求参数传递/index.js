const express = require('express')
const app = express()
const homeRouter = require('./router/homeRouter')
const loginRouter = require('./router/loginRouter')

app.listen(3100, ()=>{
    console.log('app start')
})

/// 解析 post 参数
app.use(express.urlencoded({extended:false}))
app.use(express.json())

// 应用级中间件: 挂在 App 上
app.use('/login', loginRouter)
app.use('/home', homeRouter)

// 错误中间件
app.use( (err, req, res, next)=>{
    console.error('错误中间件：' ,err.stack)
    res.status(500).send('Something broke!')
})
