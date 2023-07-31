const express = require('express') 

const app = express()

app.use((req, res, next)=>{

    console.log('----------- 1-1 -----------')
    next()
    console.log('----------- 1-2 -----------')
    res.send({
        name: '张三',
        age : 15,
        sex: true
    })
})

app.use((req, res, next)=>{
    console.log('----------- 2-1 -----------')
})

app.listen(3000)
