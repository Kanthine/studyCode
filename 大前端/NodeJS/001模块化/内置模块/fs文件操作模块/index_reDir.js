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

fs.writeFile('./avart/1.txt', "Hello Word!!\n").then(data=>{
    console.log('writeFile data:', data)
}).catch(error=>{
    if(eerrorrr && error.code == 'ENOENT') {
        console.log('目录不存在，无法打开')
    } else {
        console.log('error:', error)
    }  
})

/// 删除文件
fs.readdir('./avart').then(async data=>{
    await Promise.all(data.map(item=>fs.unlink(`./avart/${item}`)))
    await fs.rmdir('./avart' )
}).catch(error=>{

})