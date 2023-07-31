const Router = require('koa-router') 
const router = new Router()

router.get('/', async (ctx, next)=>{
    await ctx.render('home', {title: 'koaTest'}) // 自动查找 views/home.ejs
})

module.exports= router;


/**
 * cookie 
 *      ctx.cookies.get('koa-age')
 *      ctx.cookies.set('koa-age', 'kooa')
*/