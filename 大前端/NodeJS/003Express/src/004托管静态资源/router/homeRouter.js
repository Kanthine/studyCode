const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    // res.send('home')
    res.send('<h1>HOME</h1>')
    // res.render(`HOME`)
})

router.get('/a', (req, res)=>{
    res.send('home a')
})

router.get('/b', (req, res)=>{
    res.send('home b')
})

module.exports= router;