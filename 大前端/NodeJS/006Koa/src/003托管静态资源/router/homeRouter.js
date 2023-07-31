const Router = require('koa-router') 
const router = new Router()

// 路由级中间件
router.get('/', (ctx, next)=>{
    ctx.response.body = `
    <h1>Home</h1>
    `
})

module.exports= router;