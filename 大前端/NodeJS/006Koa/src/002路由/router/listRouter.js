const Router = require('koa-router') 
const router = new Router()

// 路由级中间件
router.get('/', (ctx, next)=>{
    ctx.response.body = {
        name: '李四',
        age : 25,
        sex: true
    }
})

module.exports= router;