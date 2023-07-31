const Router = require('koa-router') 
const router = new Router()

// 路由级中间件
router.get('/', (ctx, next)=>{
    console.log('query:', ctx.request.query, ctx.request.querystring)

    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

router.post('/', (ctx, next)=>{
    // console.log('query:', ctx.request.query, ctx.request.querystring)

    ctx.response.body = {
        name: '张三 post',
        age : 15,
        sex: true
    }
})

module.exports= router;