function common() {
    console.log('公共方法 a')
}

function initA() {
    console.log('initA')
}



// module.exports.common = common
// module.exports.initA = initA

// 两种导出方式都可以
module.exports = {common, initA}