const koa = require('koa') 

const app = new koa()

app.use((ctx, next)=>{

    console.log('----------- 1-1 -----------')
    next()
    console.log('----------- 1-2 -----------')
    ctx.response.body = {
        name: '张三',
        age : 15,
        sex: true
    }
})

app.use((ctx, next)=>{
    console.log('----------- 2-1 -----------')
})

app.listen(3000)
