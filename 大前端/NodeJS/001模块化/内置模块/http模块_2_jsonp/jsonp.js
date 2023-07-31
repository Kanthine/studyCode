const HTTP = require('http')
const URL = require('url')

HTTP.createServer((request, response)=>{
    let urlObj = URL.parse(request.url, true)
    console.log(urlObj.query.callback)
    switch(urlObj.pathname) {
        case '/api/user': {
            let val = `${urlObj.query.callback}(${
                JSON.stringify({
                    name: 'Tom',
                    age: 18,
                    sex: true
                })
            })`
            response.end(val)
        }break;
        default: 
            response.end('404') 
            break;
    }
}).listen(3100, ()=>{
    console.log('jsonp start')
})