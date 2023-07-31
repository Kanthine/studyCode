const express = require('express')
const router = express.Router()

// 路由级中间件
router.get('/', (req, res)=>{
    console.log('-------home-------')
    // 渲染模版后返回给前端
    res.render('home', {title:'后端模板', list:[
        'a',
        'b',
        'c',
        'd',
    ]}) /// 去 views 文件夹下寻找 home.ejs
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