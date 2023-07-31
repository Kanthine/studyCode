const koa = require('koa') 
const Static = require('koa-static') 
const BodyParser = require('koa-bodyparser')
const Path = require('path')
const views = require('koa-views')
const session = require('koa-session-minimal')

const app = new koa()
const router = require('./router/router')
const JWT = require('./unit/JWT')

app.use(BodyParser()) /// 解析 post 参数
app.use(Static(Path.join(__dirname, 'static')))
app.use(views(Path.join(__dirname, 'views'), {extension:'ejs'}))


app.use(session({
    key:"koa-test",
    cookie:{
        maxAge:1000*60*60
    }
}))

// session 拦截
app.use(async (ctx, next)=>{
    console.log('session', ctx.session.user)
    if(ctx.url.includes('login')) {
        await next()
        return
    }

    const token = ctx.headers["authorization"]?.split(" ")[1]
    if(token) { // 校验 token  
        const paylod = JWT.verify(token)
        if (paylod) {
            var info = {_id: paylod._id, username: paylod.username}
            const newToken = JWT.generate(info, '1d')
            ctx.set('Authorization', newToken)
            await next()
        } else {
            ctx.status = 401
            ctx.response.body = {errCode:-1, message:'token 过期'}
        }
    } else {
        await next()
    }
})

app.use(router.routes()).use(router.allowedMethods())
app.listen(3000)


/** npm install koa-router
 *  npm install koa-static
 *  npm install koa-bodyparser
 *  npm install ejs
 *  npm install koa-views
 *  npm install koa-session-minimal
 */