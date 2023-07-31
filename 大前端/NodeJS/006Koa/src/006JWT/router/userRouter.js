const Router = require('koa-router') 
const JWT = require('../unit/JWT')
const router = new Router()

// 路由级中间件
router.get('/', (ctx, next)=>{
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

router.post('/', (ctx, next)=>{
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

router.get('/login', (ctx, next)=>{
    console.log('query:', ctx.request.query, ctx.request.querystring)

    var info = {_id: '111111', username:  ctx.request.query.username}
    const token = JWT.generate(info, '1d')
    ctx.set('Authorization', token)
    
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

router.post('/login', (ctx, next)=>{
    console.log('body:', ctx.request.body)
    
    var info = {_id: '111111', username:  '张三'}
    const token = JWT.generate(info, '1d')
    ctx.set('Authorization', token)

    ctx.response.body = {
        name: '张三 post',
        age : 15,
        sex: true
    }
})
module.exports= router;