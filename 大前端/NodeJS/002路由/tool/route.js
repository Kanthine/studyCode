const fs = require('fs')
const path = require('path')

/// 通过文件扩展名、获取文件类型 Content-Type
const mime = require('mime')

function render(res, path, type="") {
    res.writeHead(200, {
        'Content-Type':`${type?type:'text/html'};charset=utf-8`,
    })
    res.write(fs.readFileSync(path), 'utf-8')
    res.end()
}

/// 静态渲染时，静态服务器通过静态路由，使用相对路径，因此文件都可以加载到
/// 动态渲染时，node 服务器在 http://localhost/ 的绝对路径下查找资源，找到资源并返回给 node 服务器
function readStaticFile(req, res) {
    const myURL = new URL(req.url, 'http:127.0.0.1')    
    if(myURL.pathname == '/') return false

    let dir = __dirname
    dir = dir.replace('tool', 'static')
    const pathName = path.join(dir, myURL.pathname)
    if(fs.existsSync(pathName)) {
        let type = myURL.pathname.split('.')[1] /// 文件扩展名 js、css 等
        render(res, pathName, mime.getType(type))
        return true
    } else {
        return false
    }
}

const router = {
    '/': (req, res)=>render(res, '../static/index.html'),
    '/login': (req, res)=>render(res, '../static/login.html'),
    '/home': (req, res)=>render(res, '../static/home.html'),
    '/404': (req, res)=>{

        if(readStaticFile(req, res)) {
            return
        }

        res.writeHead(404, {
            'content-type':'text/html;charset=utf-8',
        })
        res.write(fs.readFileSync('../static/404.html'), 'utf-8')
        res.end()
    }
}

module.exports = router;


// function router1(res, pathname){
//     console.log(pathname)
//     switch(pathname) {
//         case '/login':{
//             res.writeHead(200, {
//                 'Content-Type':'text/html:charset=utf-8',
//             })
//             res.write(fs.readFileSync('../login.html'), 'utf-8')
//         }break;
//         case '/home':{
//             res.writeHead(200, {
//                 'content-type':'text/html:charset=utf-8',
//             })
//             res.write(fs.readFileSync('../home.html'), 'utf-8')
//         }break;
//         default: {
//             res.writeHead(404, {
//                 'content-type':'text/html:charset=utf-8',
//             })
//             res.write(fs.readFileSync('../404.html'), 'utf-8')
//         }break;
//     }
// }
