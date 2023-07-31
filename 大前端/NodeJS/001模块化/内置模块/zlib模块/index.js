const HTTP = require('http')
const fs = require('fs')
const zlib = require('zlib')

const gzip = zlib.createGzip()

HTTP.createServer((req, res) => { // res 可写流
    const rs = fs.createReadStream('./index.js')
    res.writeHead(200, {
        'Content-Type': 'text/html;charset=utf-8',
        'Content-Encoding': 'gzip'  /// 必须设置解压信息
    })
    rs.pipe(gzip).pipe(res) /// 压缩
}).listen(3100, ()=>{
    console.log('server start')
})