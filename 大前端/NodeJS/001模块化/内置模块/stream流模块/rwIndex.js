const fs = require('fs')

/// 读取流
const rs = fs.createReadStream('./rwIndex.js', 'utf-8')
/// 写入流
const ws = fs.createWriteStream('./1.tex', 'utf-8')
/// 将读取流送入写入流
rs.pipe(ws) /// 边读边写: 适用于大文件的复制