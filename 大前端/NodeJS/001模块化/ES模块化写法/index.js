import {common, initA} from './module/moduleA.js'
import moduleB from './module/moduleB.js'

moduleB()
initA()
common()


/* /// package.json 文件
{
  "name": "test",
  "version": "1.0.0",
  "main": "index.js",
  "type": "module",  使用 ES6 引入模方式。默认是 commonjs 语法规范；两种规范不能混着用
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  }
}
*/


/**
 *  ES 模块化 导出函数或者类的几种写法：
 * 1、导出一个字典      export {common, initA}
 * 2、导出一个类或函数  export default common
*/