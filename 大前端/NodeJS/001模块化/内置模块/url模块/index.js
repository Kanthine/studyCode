/// URL文档：http://nodejs.cn/api/url.html 
const url = require('url')

/// 解析 URL 地址
function parserURL(urlStr) {
    /// parse 解析 URL 地址
    const myURL = new URL(urlStr)
    console.log(myURL)
    for (const obj of myURL.searchParams) {
        console.log(obj)
    }
    /*
        URL {
            href: 'https://www.test.com:8081/api/user?age=18&sex=true',
            origin: 'https://www.test.com:8081',
            protocol: 'https:',
            username: '',
            password: '',
            host: 'www.test.com:8081',
            hostname: 'www.test.com',
            port: '8081',
            pathname: '/api/user',
            search: '?age=18&sex=true',
            searchParams: URLSearchParams { 'age' => '18', 'sex' => 'true' },
            hash: ''
        }
    */
}
parserURL('https://www.test.com:8081/api/user?age=18&sex=true')

let obj = {
    protocol: 'https:',
    slashes: true,
    host: 'www.test.com:8888',
    port: '8888',
    hostname: 'www.test.com',
    search: '?age=18&sex=true',
    query: { age: '18', sex: 'true' },
    pathname: '/api/user',
    path: '/api/user?age=18&sex=true',
}

/// 组装 URL 地址
let aURL = url.format(obj)
/// https://www.test.com:8888/api/user?age=18&sex=true


let urlStr1 = new URL('4', 'https://www.test.com/1/2/3')     /// https://www.test.com/1/2/4
let urlStr2 = new URL('/4', 'https://www.test.com/1/2/3')    /// https://www.test.com/4
let urlStr3 = new URL('4', 'https://www.test.com/1/2/3/')    /// https://www.test.com/1/2/3/4
console.log(urlStr1.href)
console.log(urlStr2.href)
console.log(urlStr3.href)