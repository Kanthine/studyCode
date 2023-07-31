function common() {
    console.log('公共方法 a')
}

function initA() {
    console.log('initA')
}

// 两种导出方式都可以
export {common, initA}
