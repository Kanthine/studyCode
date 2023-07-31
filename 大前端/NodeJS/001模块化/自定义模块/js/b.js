/// 何处使用、何处导入
const moduleA = require('./a') /// 导入依赖文件

function common() {
    console.log('公共方法 b')
}

function initB() {
    moduleA.initA()
    console.log('initB 依赖于 initA')
}

module.exports = {common, initB}