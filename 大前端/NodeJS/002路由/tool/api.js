const fs = require('fs')

function render(res, data, type="") {
    console.log('Content-Type: ', `${type?type:'application/json'};charset=utf-8`)
    res.writeHead(200, {
        'Content-Type':`${type?type:'application/json'};charset=utf-8`,
    })
    res.write(data)
    res.end()
}

const apiRouter = {
    '/api/login': (req, res)=> {
        const myURL = new URL(req.url, 'http:127.0.0.1')        
        let username = myURL.searchParams.get('username')
        let passowrd = myURL.searchParams.get('passowrd')
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
        console.log('username: ', JSON.stringify(data))
        render(res, JSON.stringify(data))
    },
    '/api/loginpost': (req, res)=> {
        
        let reqData = ''
        req.on('data', chunk=>{
            reqData += chunk
        })
        req.on('end', ()=>{
            reqData = JSON.parse(reqData)
            let username = reqData.username
            let password = reqData.password
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
            render(res, JSON.stringify(data))
        })
    }
}

module.exports = apiRouter;