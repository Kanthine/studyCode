const HTTP = require('http')
const HTTPS = require('https')
const URL = require('url')

HTTP.createServer((request, response)=>{
    let urlObj = URL.parse(request.url, true)
    switch(urlObj.pathname) {
        case '/api/list': {
            getList((data)=>{
                response.writeHead(200,{
                    'Content-Type': 'application/json;charset=utf-8',
                    'access-control-allow-origin': '*'
                })
                response.end(data)
            })
        }break;
        default: 
            response.end('404') 
            break;
    }
}).listen(3100, ()=>{
    console.log('get start')
})


// 中间件：去别的服务器请求数据
function getList(cb) {
    let data = ''
    let urlStr = 'https://i.maoyan.com/api/mmdb/movie/v3/list/hot.json?ct=%E4%B8%8A%E6%B5%B7&ci=10&channelId=4';
    HTTPS.get(urlStr, (res)=>{
        res.on('data', (chunk)=>{
            data += chunk
        })
        res.on('end', ()=>{
            cb(data)
        })
    })
}