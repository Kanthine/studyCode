const fs = require('fs')

/// 读取流
const rs = fs.createReadStream('./index.js', 'utf-8')

rs.on('data',(chunk)=>{
    console.log('chunk:', chunk)
})

rs.on('end',()=>{
    console.log('======end')
})

rs.on('error',(error)=>{
    console.log('error', error)
})

/// 写入流
const ws = fs.createWriteStream('./1.tex', 'utf-8')
ws.write('11111111111 \n')
ws.write('22222222222 \n')
ws.write('33333333333 \n')
ws.end()