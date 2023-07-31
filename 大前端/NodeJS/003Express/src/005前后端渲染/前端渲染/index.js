const express = require('express')
const app = express()
const homeRouter = require('./router/homeRouter')


app.listen(3100, ()=>{
    console.log('app start')
})

/// 解析 post 参数
app.use(express.urlencoded({extended:false}))
app.use(express.json())

app.use(express.static('static')) /// 配置静态资源
app.use('/home', homeRouter)

// 错误中间件
app.use( (err, req, res, next)=>{
    console.error('错误中间件：' ,err.stack)
    res.status(500).send('Something broke!')
})
