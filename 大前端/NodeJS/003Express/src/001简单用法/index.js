const express = require('express')
const app = express()

app.listen(3100, ()=>{
    console.log('app start')
})

// ********************************* 使用字符串模式的路由路径示例 *********************************

app.get('/', (req, res)=>{
    /// send() 封装了 res.write() 与 res.end() 的功能
    res.send(`<h1>Hello Word!</h1>`)
})

app.get('/login', (req, res)=>{
    res.send({
        username:'张三',
        age: 18,
        sex: true
    })
})

// 匹配 acd 和 abcd
app.get('/ab?cd', (req, res)=>{
    res.send('Hello ab?cd!')
})

// 匹配 /qw/******
app.get('/qw/:id/:id', function(req, res) {
    res.send('qw');
});

// 匹配 abcd、abbcd、abbbcd等 
app.get('/ab+cd', function(req, res) {
    res.send('ab+cd');
});

// 匹配 abcd、abxcd、abRABDOMcd、ab123cd等 
app.get('/ab*cd', function(req, res) {
    res.send('ab*cd');
});

// 匹配 /abe 和 /abcde 
app.get('/ab(cd)?e', function(req, res) {
    res.send('ab(cd)?e');
});

// 正则表达式: 匹配任何路径中含有 vb 的路径
app.get(/vb/, function(req, res) {
    res.send('/vb/');
});

// 正则表达式: 匹配以 fly 结尾的路径
app.get(/.*fly$/, function(req, res) {
    res.send('/.*fly$/');
});


// ********************************* 使用字符串模式的路由路径示例 *********************************
// 可以为请求处理提供多个回调函数，其行为类似中间件。
// 唯一的区别是这些回调函数有可能调用 next('route') 方法而略过其他路由回调函数。
// 可以利用该机制为路由定义前提条件，如果在现有路径上 继续执行没有意义，则可将控制权交给剩下的路径。

// 写法一：
// app.get('/resetInfo/step', (req, res) => {
//     res.send('resetInfo 验证 token!');
//     // next();
// }, (req, res) => {
//     res.send('resetInfo 上传信息!');
//     // next();
// }, (req, res) => {
//     res.send('resetInfo 修改成功!');
//     // next();
// });

// 写法二：使用回调函数数组处理路由
const resetToken=(req, res, next)=>{
    console.log('resetInfo 验证 token!');
    if (true) {
        /// 走向下一个节点
        res.tokenInfo = 'token'
        next()
    } else {
        /// 结束整个流程
        res.send('resetInfo token 失败!');
    }
}

const resetUp=(req, res, next)=>{
    if (!res.tokenInfo) {
        res.send('resetInfo token 信息缺失!');
    } else {
        console.log('resetInfo 上传信息!');
        next()
    }
}

const resetSu=(req, res, next)=>{
    console.log('resetInfo 修改成功!');
    next()
}

app.get('/resetInfo/step', [resetToken, resetUp, resetSu], (req, res)=>{
    res.send('resetInfo 结束!');
})