const koa = require('koa') 
const Static = require('koa-static') 
const BodyParser = require('koa-bodyparser')
const Path = require('path')
const views = require('koa-views')

const app = new koa()
const router = require('./router/router')

app.use(BodyParser()) /// 解析 post 参数
app.use(Static(Path.join(__dirname, 'static')))
app.use(views(Path.join(__dirname, 'views'), {extension:'ejs'}))
app.use(router.routes()).use(router.allowedMethods())
app.listen(3000)


/** npm install koa-router
 *  npm install koa-static
 *  npm install koa-bodyparser
 *  npm install ejs
 *  npm install koa-views
 */