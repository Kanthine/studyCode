const HTTP = require('http')
const HTTPS = require('https')
const URL = require('url')
const EventEmitter = require('events')


let event = null
HTTP.createServer((request, response)=>{
    let urlObj = URL.parse(request.url, true)

    response.writeHead(200,{
        'Content-Type': 'application/json;charset=utf-8',
        'access-control-allow-origin': '*'
    })

    switch(urlObj.pathname) {
        case '/api/list': {
            event = new EventEmitter()
            event.on('play', (data) => {
                console.log('data:', data)
                response.end(data)
            })
            getList()
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
    console.log('get/post start')
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

function getList() {
    let data = ''
    let urlStr = 'https://i.maoyan.com/api/mmdb/movie/v3/list/hot.json?ct=%E4%B8%8A%E6%B5%B7&ci=10&channelId=4';
    HTTPS.get(urlStr, (res)=>{
        res.on('data', (chunk)=>{
            data += chunk
        })
        res.on('end', ()=>{
            event.emit('play', data)
        })
    })
}