const Router = require('koa-router') 
const router = new Router()

// 路由级中间件
router.get('/login', (ctx, next)=>{
    console.log('query:', ctx.request.query, ctx.request.querystring)
    ctx.session.user = {
        name: '张三',
        age : 15,
        sex: true
    }
    
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

router.post('/login', (ctx, next)=>{
    console.log('body:', ctx.request.body)

    ctx.session.user = {
        name: '张三 post',
        age : 15,
        sex: true
    }

    ctx.response.body = {
        name: '张三 post',
        age : 15,
        sex: true
    }
})
module.exports= router;