const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    res.send('list')
})

router.get('/a', (req, res)=>{
    res.send('list a')
})

router.get('/b', (req, res)=>{
    res.send('list b')
})

module.exports= router;