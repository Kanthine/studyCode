const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    res.send('list')
})

router.get('/login', (req, res)=>{
    let {username, passowrd} = req.query
    let data = {code: 1}
    if (username && passowrd) {
        data = {
            code: 0,
            user: {
                username:username,
                age:18,
                sex: true
            }
        }
    }
    // console.log('loginInfo: ', JSON.stringify(data))
    res.send(JSON.stringify(data))
})

router.post('/loginpost', (req, res)=>{
    let {username, password} = req.body
    let data = {code: 1}
    if (username && password) {
        data = {
            code: 0,
            user: {
                username:username,
                age:18,
                sex: true
            }
        }
    }
    // console.log('login post Info: ', JSON.stringify(data))
    res.send(JSON.stringify(data))
})

module.exports= router;