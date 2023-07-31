const koa = require('koa') 
const app = new koa()

app.use((ctx, next)=>{

    // ctx.response.body = `<h1>Hello word!</h1>`

    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

app.listen(3000)

/* npm install koa
 *
 * koa 是由 Express 原班人马打造的，更小、更富有表现力、更健壮的 web 框架
 * 使用 koa 编写 web 应用，通过组合不同的 generator，可以免费重复繁琐的回调函数嵌套，并极大提升错误处理的效率；
 * koa 不在内核中绑定任何中间件，它仅仅提供了一个轻量优雅的函数库，使得编写 web 得心应手；
 * 
 * 
 * 
 * koa 更加的轻量级：
 *     不提供内置中间件；
 *     不提供路由，将路由苦分离出来 koa/router
 * koa 提供了 context 对象
 *     context 作为请求的上下文对象，挂载了 request 和 response 两个对象
 * koa 异步流程控制
 *     express 采用 callback 来处理异步，koa v1 采用 generator， koa v2 采用 async/await
 *     generator 和 async/await 使用同步的方法来处理异步，明显好于 callback 和 promise
 * 
*/