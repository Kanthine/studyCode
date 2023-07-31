function common() {
    console.log('公共方法 c')
}

/// 只要不导出，就是私有的
function _initC() {
    console.log('initC 私有方法')
}

module.exports = common