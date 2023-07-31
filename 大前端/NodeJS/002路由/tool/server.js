const HTTP = require('http')

const Router = {}
function use(obj) {
    Object.assign(Router, obj)
}

function start() {
    HTTP.createServer((req, res)=>{
        const myURL = new URL(req.url, 'http:127.0.0.1')
        try{
            Router[myURL.pathname](req, res);
        }catch (error){
            Router['/404'](req, res);
        }
        
    }).listen(3100, ()=>{
        console.log('server start')
    })
}

exports.server = {start, use};
