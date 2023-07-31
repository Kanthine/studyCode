const crypto = require('crypto')

const md5T = crypto.createHash('md5')
md5T.update('Hello word!')
console.log('md5T: ', md5T.digest('hex'))


const sha1T = crypto.createHash('sha1')
sha1T.update('Hello word!')
console.log('sha1T: ', sha1T.digest('hex'))


const hmac = crypto.createHmac('sha256', 'custom secret key');  /// 自定义密匙
hmac.update('Hello, world!');
console.log('hmac: ', hmac.digest('hex'))

/// AES是一种常用的对称加密算法，加解密都用同一个密钥。crypto模块提供了AES支持，但是需要自己 封装好函数，便于使用:
function encrypt (key, iv, data) { /// 加密
    let decipher = crypto.createCipheriv('aes-128-cbc', key, iv);
    return decipher.update(data, 'binary', 'hex') + decipher.final('hex');
}
function decrypt (key, iv, crypted) { /// 解密
    crypted = Buffer.from(crypted, 'hex').toString('binary');
    let decipher = crypto.createDecipheriv('aes-128-cbc', key, iv);
    return decipher.update(crypted, 'binary', 'utf8') + decipher.final('utf8');
}

/// 必须是 16 字节
const Key = 'ABCDEF1234567890'
const IV = 'HIJKLM1234567890'

let oldData = 'Hello Word!'
let data1 = encrypt(Key, IV, oldData)
let data2 = decrypt(Key, IV, data1)

console.log('加密结果：', data1)
console.log('解密结果：', data2)


/**
crypto模块的目的是为了提供通用的加密和哈希算法。
用纯JavaScript代码实现这些功能不是不可能， 但速度会非常慢。
Nodejs用C/C++实现这些算法后，通过cypto这个模块暴露为JavaScript接口，这样用 起来方便，运行速度也快。
*/