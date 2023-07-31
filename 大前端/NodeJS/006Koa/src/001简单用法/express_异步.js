const express = require('express') 

const app = express()

app.use(async (req, res, next)=>{
    console.log('----------- 1-1 -----------')
    await next()
    console.log('----------- 1-2 -----------')
    res.send({
        name: '张三',
        age : 15,
        sex: true
    })
})

app.use(async (req, res, next)=>{ 
    console.log('----------- 2-1 -----------')
    await delay(1000)
    console.log('----------- 2-2 -----------')
})

function delay(time) {
    return new Promise((reslove, reject)=>{
        setTimeout(reslove, time);
    })
}

app.listen(3000)
