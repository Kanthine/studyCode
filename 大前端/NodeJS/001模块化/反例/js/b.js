function common() {
    console.log('公共方法 b')
}

function initB() {
    initA()
    console.log('initB 依赖于 initA')
}