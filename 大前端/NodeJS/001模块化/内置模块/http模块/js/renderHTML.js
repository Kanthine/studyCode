function renderHTML(url) {
    switch (url) {
        case '/home':
            return `<h1>Home</h1>`
        case '/index':
            return `<h1>index</h1>`
        case '/json':
            return `{
                data:{
                    username: 张三,
                    age: 18
                }
            }`
        default:
           return `<h1>404 NotFound</h1>`
    }
}

module.exports = {renderHTML};