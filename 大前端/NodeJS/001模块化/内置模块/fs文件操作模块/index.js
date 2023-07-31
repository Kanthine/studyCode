const fs = require('fs')

/// 创建目录(异步)
fs.mkdir('./avart', (err)=>{
    if(err && err.code == 'EEXIST') {
        console.log('目录已存在，无法创建')
    } else {
        console.log('err1:', err)
    }
})

/// 创建目录(同步)
// try {
//     fs.mkdirSync('./avart1')
// } catch(error) {
//     console.log('error:', error)
// }

/// 修改目录名字
// fs.rename('./avart', './avartNew', err=>{
//     if(err && err.code == 'ENOENT') {
//         console.log('原目录不存在，无法修改')
//     } else {
//         console.log('err2:', err)
//     }  
// })

/// 查看信息
// fs.stat('./avart', (err, data)=>{
//     if(!err) {
//         console.log('data info:', data)
//         console.log('data isFile:', data.isFile())
//         console.log('data isDirectory:', data.isDirectory())
//     }else {
//         console.log('err info:', err)
//     }  
// })

/// 读取目录
// fs.readdir('./avart', (err, data)=>{
//     if(!err) {
//         console.log('data:', data)
//     }else {
//         console.log('err:', err)
//     }  
// })

/// 删除目录
// fs.rmdir('./avart', err=>{
//     if(err && err.code == 'ENOENT') {
//         console.log('目录不存在，无法删除')
//     } else if(err && err.code == 'ENOTEMPTY') {
//         console.log('目录中存在文件，无法删除')
//     } else {
//         console.log('err3:', err)
//     }  
// })

/// 向指定文件写入内容，会覆盖原有内容 
// fs.writeFile('./avart/1.txt', "Hello Word!!\n", err=>{
//     if(err && err.code == 'ENOENT') {
//         console.log('目录不存在，无法打开')
//     } else {
//         console.log('err4:', err)
//     }  
// })

/// 向指定文件继续追加内容: 没有文件则创建文件
// fs.appendFile('./avart/1.txt', "Hello Word2!!\n", err=>{
//     console.log('err5:', err)
// })

/// 读取文件
// fs.readFile('./avart/1.txt', (err, data)=>{
//     console.log('err6:', err)
//     let string = data.toString('utf-8')
//     console.log('data6:', string)
// })

/// 删除文件
// fs.unlink('./avart/1.txt', err => {
//     if(err && err.code == 'ENOENT') {
//         console.log('目录或文件不存在，无法删除')
//     } else {
//         console.log('err7:', err)
//     }  
// })