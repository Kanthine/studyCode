const http = require('http')
const url = require('url')
const rHTML = require('./renderHTML')
const rStatus = require('./renderStatus')

/// 创建服务器: 写法一
// http.createServer((request, response)=>{
//     /// request  浏览器的传参
//     /// response 返回渲染的内容
//     if (request.url == '/favicon.ico') {
//         return
//     }
//     response.writeHead(rStatus.renderStatus(url), {
//         'Content-Type': 'text/html;charset=utf-8',
//         'access-control-allow-origin': '*' 
//     })
//     response.write('<h1>Hello Word</h1>')
//     response.end(rHTML.renderHTML(request.url))
// }).listen(3100, ()=>{ /// 监听指定端口号
//     console.log('server start')
// })


/// 创建服务器: 写法二
const server = http.createServer()
server.listen(3100, ()=>{
    console.log('server start')
})
server.on('request', (request, response)=>{

    console.log('url', request.url)

    /// parse 解析 URL 地址
    let urlInfo = url.parse(request.url, true)
    console.log('info', urlInfo)
    let pathName = urlInfo.pathname;
    let query = urlInfo.query;
    console.log('query', query)
    if (pathName== '/favicon.ico') {
        return
    }
    let status = rStatus.renderStatus(pathName)
    response.writeHead(status, {
        'Content-Type': 'text/html;charset=utf-8',
        'access-control-allow-origin': '*' 
    })
    response.end(rHTML.renderHTML(pathName))
}) 