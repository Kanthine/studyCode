const koa = require('koa') 

const app = new koa()

app.use(async (ctx, next)=>{

    console.log('----------- 1-1 -----------')
    await next()
    console.log('----------- 1-2 -----------')
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

app.use(async (ctx, next)=>{
    console.log('----------- 2-1 -----------')
    await delay()
    console.log('----------- 2-2 -----------')
})

function delay(time) {
    return new Promise((reslove, reject)=>{
        setTimeout(reslove, time);
    })
}

app.listen(3000)
