const HTTP = require('http')
const HTTPS = require('https')
const URL = require('url')
const CHEERIO = require('cheerio')

HTTP.createServer((request, response)=>{
    let urlObj = URL.parse(request.url, true)

    response.writeHead(200,{
        'Content-Type': 'application/json;charset=utf-8',
        'access-control-allow-origin': '*'
    })

    switch(urlObj.pathname) {
        case '/api/list': {
            getList((data)=>{
                response.end(data)
            })
        }break;
        case '/api/goods' : {
            postList((data)=>{
                response.end(data)
            })
        }
        break;
        default: 
            response.end('404') 
            break;
    }
}).listen(3100, ()=>{
    console.log('爬虫 start')
})

function postList(cb) {
    let options = {
        hostname: 'm.xiaomiyoupin.com',
        port: '443',
        path: '/mtop/market/search/placeHolder',
        method: 'POST',
        headers: {
            'Content-type': 'application/json; charset=utf-8',
        }
    }
    let data = ''
    let request = HTTPS.request(options, (res)=>{
        res.on('data', (chunk)=>{
            data += chunk
        })
        res.on('end', ()=>{
            cb(data)
        })
    })
    request.write(JSON.stringify([{},{"baseParam":{"ypClient":1}}]))
    request.end()
}

function getList(cb) {
    let data = ''
    let urlStr = 'https://m.lingdiankanshu.co';
    // urlStr = 'https://m.lingdiankanshu.co/504003/8323830.html';
    HTTPS.get(urlStr, (res)=>{
        res.on('data', (chunk)=>{
            data += chunk
        })
        res.on('end', ()=>{
            console.log('data:', data)
            // spider(data)
            cb(JSON.stringify('hello'))
        })
    })
}

// cheerio 模块：解析 HTML 结构
function spider(data) {
    let $ = CHEERIO.load(data);
    let $list = $('.layout-col2')
    console.log('$list' ,$)
}