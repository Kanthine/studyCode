const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    res.send({
        code:0
    })
})

router.get('/list', (req, res)=>{
    let list = [
        {
            name:'第一件商品',
            price: 18.5
        },
        {
            name:'第二件商品',
            price: 58.7
        }
    ]
    res.send(JSON.stringify(list))
})

module.exports= router;