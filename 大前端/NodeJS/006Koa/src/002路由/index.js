const koa = require('koa') 
const app = new koa()
const router = require('./router/router')
/// router.allowedMethods()
app.use(router.routes()).use(router.allowedMethods())
app.listen(3000)


/** npm install koa-router
 * 
 * 
*/