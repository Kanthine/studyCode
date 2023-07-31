const express = require('express')
const app = express()
const homeRouter = require('./router/homeRouter')
const loginRouter = require('./router/loginRouter')


app.listen(3100, ()=>{
    console.log('app start')
})

/// 配置模板引擎
app.set('views', './views')
// app.set('view engine', 'ejs')

/// 支持渲染 html 文件
app.set('view engine', 'html')
app.engine('html', require('ejs').renderFile)  

/// 解析 post 参数
app.use(express.urlencoded({extended:false}))
app.use(express.json())

app.use(express.static('static')) /// 配置静态资源
app.use('/home', homeRouter)
app.use('/login', loginRouter)

// 错误中间件
app.use( (err, req, res, next)=>{
    console.error('错误中间件：' ,err.stack)
    res.status(500).send('Something broke!')
})
