const koa = require('koa') 
const Static = require('koa-static') 
const BodyParser = require('koa-bodyparser')
const Path = require('path')
const views = require('koa-views')
const session = require('koa-session-minimal')

const app = new koa()
const router = require('./router/router')

app.use(BodyParser()) /// 解析 post 参数
app.use(Static(Path.join(__dirname, 'static')))
app.use(views(Path.join(__dirname, 'views'), {extension:'ejs'}))

app.use(session({
    key: 'koa_test',
    cookie: {
        maxAge: 1000 * 60 * 60,
    }
}))

// session 拦截
app.use(async (ctx, next)=>{
    console.log('session', ctx.session.user)
    if(ctx.url.includes('login')) {
        await next()
    } else if(ctx.session.user) { // 校验 session  
        ctx.session.date = Date.now() /// 重置过期时间
        await next()  
    } else {
        ctx.redirect('/login')  
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