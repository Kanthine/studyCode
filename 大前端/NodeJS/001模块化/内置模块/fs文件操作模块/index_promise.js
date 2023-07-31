const fs = require('fs').promises

/// 创建目录
fs.mkdir('./avart').then(data=>{
    console.log('creat:', data)
}).catch(error=>{
    if(error && error.code == 'EEXIST') {
        console.log('目录已存在，无法创建')
    } else {
        console.log('error:', error)
    }  
})

/// 修改目录名字
// fs.rename('./avart', './avartNew').then(data=>{
//     console.log('rename:', data)
// }).catch(error => {
//     if(err && err.code == 'ENOENT') {
//         console.log('原目录不存在，无法修改')
//     } else {
//         console.log('err2:', err)
//     }  
// })

/// 查看信息
// fs.stat('./avart').then(data=>{
//     console.log('data info:', data)
//     console.log('data isFile:', data.isFile())
//     console.log('data isDirectory:', data.isDirectory())
// }).catch(error=>{
//     console.log('error info:', error)
// })

/// 读取目录
// fs.readdir('./avart').then(data=>{
//     console.log('readdir data:', data)
// }).catch(error=>{
//     console.log('err:', err)
// })

/// 删除目录
// fs.rmdir('./avartNew').then(data=>{
//     console.log('rmdir data:', data)
// }).catch(error=>{
//     if(error && error.code == 'ENOENT') {
//         console.log('目录不存在，无法删除')
//     } else if(error && error.code == 'ENOTEMPTY') {
//         console.log('目录中存在文件，无法删除')
//     } else {
//         console.log('error:', error)
//     }  
// })

/// 向指定文件写入内容，会覆盖原有内容 
// fs.writeFile('./avart/1.txt', "Hello Word!!\n").then(data=>{
//     console.log('writeFile data:', data)
// }).catch(error=>{
//     if(eerrorrr && error.code == 'ENOENT') {
//         console.log('目录不存在，无法打开')
//     } else {
//         console.log('error:', error)
//     }  
// })


/// 向指定文件继续追加内容: 没有文件则创建文件
// fs.appendFile('./avart/1.txt', "Hello Word2!!\n").then(data=>{
//     console.log('appendFile data:', data)
// }).catch(error=>{
//     console.log('error:', error)
// })

/// 读取文件
// fs.readFile('./avart/1.txt').then(data=>{
//     let string = data.toString('utf-8')
//     console.log('readFile data:', string)
// }).catch(error=>{
//     console.log('error:', error)
// })

/// 删除文件
// fs.unlink('./avart/1.txt').then(data=>{
//     console.log('unlink data:', data)
// }).catch(error=>{
//     if(error && error.code == 'ENOENT') {
//         console.log('目录或文件不存在，无法删除')
//     } else {
//         console.log('error:', error)
//     }  
// })


/*
promises 写法：使用  .then .catch 等

*/