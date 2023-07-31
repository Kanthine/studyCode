function common() {
    console.log('公共方法 c')
}

function initC() {
    initB()
    console.log('initC 依赖于 initB')
}