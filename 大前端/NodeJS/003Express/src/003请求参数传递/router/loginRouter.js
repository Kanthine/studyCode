const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    console.log('get', req.query)
    res.send({
        code: 0,
        data:{
            name:'张三',
            age: 20
        }
    })
})

router.post('/', (req, res)=>{
    console.log('post:', req.body)
    res.send({
        code: 0,
        data:{
            name:'李四',
            age: 30
        }
    })
})

module.exports= router;