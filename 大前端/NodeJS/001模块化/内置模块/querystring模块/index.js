const Tooler = require('querystring')

var str = 'name=nick&age=10&sex=true'
var info = Tooler.parse(str)    /// 将字符串解析为 map：{ name: 'nick', age: '10', sex: 'true' }

var obj = {
    name:'Tom',
    age: 28,
    sex: false
}
var objStr = Tooler.stringify(obj)  /// 将 map 转为字符串  name=Tom&age=28&sex=false


var str2 = 'id=1&city=北京&url=https://www.test.com'
var str2_1 = Tooler.escape(str2)     /// 转译：id%3D1%26city%3D%E5%8C%97%E4%BA%AC%26url%3Dhttps%3A%2F%2Fwww.test.com
var str2_2 = Tooler.unescape(str2_1) /// 反转译 id=1&city=北京&url=https://www.test.com

console.log(info)
console.log(objStr)
console.log(str2_1)
console.log(str2_2)

