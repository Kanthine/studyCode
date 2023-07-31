[JS 教学视频](https://www.bilibili.com/video/BV15T411j7pJ?p=2&vd_source=3e79aa0ad6c6dd422df6eab7d2129711)

# 1、组成部分

JS 主要由三部分组成：
* BOM (Browser Object Model): 操作浏览器发生变化的属性和方法；
* DOM (Document Object Model): 操作文档流发生变化的属性和方法； 
* ECMAScript: JS 的书写语法与书写规则；

## 1.1、BOM操作

BOM (Browser Object Model): 操作浏览器发生变化的属性和方法：
* 操作浏览器历史记录；
* 操作浏览器滚动条；
* 操作浏览器页面跳转；
* 操作浏览器标签页的开启和关闭；

### 1.1.1、获取浏览器窗口尺寸

```
/// 浏览器窗口尺寸
window.innerWidth
window.innerHeight

/// 浏览器卷去的尺寸
document.documentElement.scrollTop
document.documentElement.scrollLeft
document.body.scrollTop /// 如果没有 <!DOCTYPE html> 标签，则使用该方法
document.body.scrollLeft

/// 兼容写法
let height = document.documentElement.scrollTop || document.body.scrollTop
let width = document.documentElement.scrollLeft || document.body.scrollLeft

/// 指定滚动位置
window.scrollTo(left, top) /// 没有过渡动画
window.scrollTo({
    left: 10,
    top: 100,
    behavior:"smooth",
})
```

### 1.1.2、浏览器的弹出层

```
window.alert('window.alert')     /// 提示框
window.confirm('window.confirm') /// 询问框      
let info = window.prompt('window.prompt')  /// 输入框
```

### 1.1.3、开启与关闭标签

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JS·开启与关闭标签</title>
</head>
<body>
    <button id="openWeb">开启</button>
    <button id="closeWeb">关闭</button>
    <script>
        openWeb.onclick = function() {
            window.open('https://www.baidu.com')
        }
        closeWeb.onclick = function() {
            window.close()
        }
    </script>
</body>
</html>
```

### 1.1.4、浏览器常见事件

```
window.onload = function(){
    console.log('资源加载完毕')
}
window.onresize = function(){
    console.log('可视尺寸改变')
}
window.onscroll = function(){
    console.log('滚动条位置改变')
}
```

### 1.1.5、浏览器的历史记录操作

```
window.history.back() /// 回退页面
window.history.forward() /// 前进页面
```


## 1.2、DOM操作

DOM (Document Object Model): 操作文档流发生变化的属性和方法；
* 操作 元素 修改样式
* 操作 元素 修改属性
* 操作 元素 改变位置
* 操作 元素 添加事件

### 1.2.1、获取元素

* 根据 id 名称获取
* 根据 class 名称获取
* 根据元素标签名称获取
* 根据选择器获取一个
* 根据选择器获取一组

```
/// 如果有，则返回（多个标签则返回第一个）；否则返回 null
let res1 = document.getElementById('id')

/// 返回一个数组集；如果没有，则返回空数组
let res2 = document.getElementsByClassName('item')

/// 返回一个 div 数组集；如果没有，则返回空数组
let res3 = document.getElementsByTagName('div')

///  如果有，则返回（多个则返回第一个）；否则返回 null
let res5 = document.querySelector('.item')
let res6 = document.querySelector('div')

/// 返回一个数组集；如果没有，则返回空数组
let res7 = document.querySelectorAll('.item') 
```

### 1.2.2、操作元素内容

* 操作元素文本内容
* 操作元素超文本内容


```
<div class="item"><p>Hello</p></div>

let res = document.getElementById('item')
res.innerText         /// 获取元素文本内容  Hello
res.innerText = Word  /// 修改元素文本内容

res.innerHTML         /// 获取元素文本内容  <p>Hello</p>
res.innerHTML = '<span>Word</span>'
```

### 1.2.3、操作元素属性

* 原生属性
* 自定义属性

```
let res = document.querySelector('.item')
res.className  /// 获取原生属性
res.id = 'box' /// 修改原生属性

res.setAttribute('sex', true)   /// 设置自定义属性
res.getAttribute('sex')         /// 获取自定义属性的值
res.removeAttribute('sex')      /// 删除自定义属性
```

### 1.2.4、操作元素样式

```
let res = document.querySelector('.item')

/// 行内样式
res.style           /// 获取行内样式
res.style.height    /// 获取行内样式·高度
res.style.width = '100px'   /// 设置行内样式
res.style.backgroundColor = 'red'

/// 非行内样式：只能查询、不能设置
window.getComputedStyle(res).width
```

## 1.3、DOM 节点操作

* 创建节点
* 插入节点
* 删除节点
* 替换节点
* 克隆节点

```
/// 创建一个 div 标签
let res = document.createElement('div')

/// 构造一个 p 标签、并插入 box 选择器中
<body>
    <div class="box"></div>
    <button id="creatElement">创造</button>
    <button id="deleteElement">删除</button>
    <script>
        creatElement.onclick = function() {
            let box = document.querySelector('.box')

            let p2 = document.createElement('p')
            p2.innerText = '构造一个 p2 标签'
            /// 将子节点放在父节点内部、并放在最后一个位置
            box.appendChild(p2)

            let p1 = document.createElement('p')
            p1.innerText = '构造一个 p1 标签'
            /// 将 p1 节点插入到父节点，并且排列到 p2 节点之前
            box.insertBefore(p1, p2)
        }

        deleteElement.onclick = function (){
            let box = document.querySelector('.box')
            let p = document.querySelector('p')
            box.removeChild(p) /// 从父节点中删除子节点
            let p2 = document.querySelector('p')
            p2.remove() /// 子节点自己删除
        }
    </script>    
</body>

/// 使用 p3 替换 p1 节点
box.replaceChild(p3, p1)
```


# 2、书写样式

* 行内式：直接将代码书写在标签上；
* 内嵌式: 不需要依赖任何行为，打开页面就会执行；
* 外链式：不需要依赖任何行为，打开页面就会执行；


```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JS·Demo</title>

    <!-- 引入外链式 JS 属性 -->
    <script src="./demo.js"></script>

    <!-- 内嵌式 -->
    <script>
        alert('Hello Word!!')
    </script>
</head>
<body>
    <!-- 行内式 -->
    <a href="javaScript:alert('Hello Word')">alert</a>
    <div onclick="alert('Hello Word')">alert</div>
    
</body>
</html>
```


```
/// JS 代码
alert('Hello Word!! 外链式')
```

# 3、变量

在程序运行过程中，保存一个中间值使用；

```
<script>
    var str = "你好呀"
    alert(str)
</script>
```

# 4、数据类型

JS 将数据类型分为两个大类：
* 基本数据类型：数值类型、字符串类型、布尔类型、空类型；
    * 字符串类型：一切以单引号或者双引号包括的内容；JS 不区分单引号与双引号；
    * 空类型：`Null`：表示有值，是一个空值；`Undefined` 表示没有值；
* 引用数据类型：

使用 `typeof` 关键字进行数据类型检测，返回数据类型；

```
<script>
    var str = "Hello Word!"
    var res = typeof str
    console.log(res); /// string

    var k1 = null
    console.log(typeof k1); /// null

    var k2
    console.log(typeof k2); /// undefined
</script>
```

## 4.1、数据类型转换

* 转数值；
* 转字符串；
* 转布尔；


### 4.1.1、转数值


* 转换方法一  `Number()`:

```
var str1 = '123';
var num1 = Number(str1); 

var str2 = 'abc123'; 
var num2 = Number(str2); /// NaN
<!-- num2 是一个数值类型，但它的值不是一个合法数字 -->
```

* 转换方法二  `parseInt()`: 逐位解析，直到不是数字的位停止；

```
var str1 = '123';
var num1 = parseInt(str1); 

var str2 = '12.3ab';
var num2 = parseFloat(str2); /// 12.3

var str3 = 'ab123'; 
var num3 = parseInt(str3); /// NaN
<!-- num3 是一个数值类型，但它的值不是一个合法数字 -->
```

### 4.1.2、转字符串

* 转换方法一  `String()`:

```
var k1 = 'Hello'
console.log(String(k1), typeof String(k1)) /// Hello string

var k2 = 12.5
console.log(String(k2), typeof String(k2)) /// 12.5 string

var k3 = true
console.log(String(k3), typeof String(k3)) /// true string

var k4 = null
console.log(String(k4), typeof String(k4)) /// null string

var k5 
console.log(String(k5), typeof String(k5)) /// undefined string
```

* 转换方法二  `.toString()`:

```
var k1 = 'Hello'
console.log(k1.toString(), typeof k1.toString()) /// Hello string

var k2 = 12.5
console.log(k2.toString(), typeof k2.toString()) /// 12.5 string

var k3 = true
console.log(k3.toString(), typeof k3.toString()) /// true string

<!-- 访问 null.toString() 错误：null 没有该属性  -->
var k4 = null
console.log(k4.toString(), typeof k4.toString())

<!-- 访问 undefined.toString()  错误： undefined 没有该属性  -->
var k5 
console.log(k5.toString(), typeof k5.toString())
```


### 4.1.3、转布尔值

* 转换方法一  `Boolean()`:
    * 转换为 false 的值： `0`、Nan、''、`null`、`undefined`;
    * 其余所有内容都会转换为 true；

```
var k1 = 'Hello'
console.log(Boolean(k1), typeof Boolean(k1)) /// true 'boolean'

var k2 = ''
console.log(Boolean(k2), typeof Boolean(k2)) /// false 'boolean'

var k3 = 12.5
console.log(Boolean(k3), typeof Boolean(k3)) /// true 'boolean'

var k4 = true
console.log(Boolean(k4), typeof Boolean(k4)) /// true 'boolean'

var k5 = null
console.log(Boolean(k5), typeof Boolean(k5)) /// false 'boolean'

var k6 
console.log(Boolean(k6), typeof Boolean(k6)) /// false 'boolean'
```

## 4.2、常用方法

### 4.2.1、字符串方法

```
let str = 'Hello, Word!'
str.charAt(0) /// 获取字符串的第 0 个索引的字符；
let str2 = str.toLocaleLowerCase() /// 字符串的所有大写转小写,返回新的字符串
let str3 = str.toLocaleUpperCase() /// 字符串的所有小写转大写,返回新的字符串
let str4 = str.replace('!', 'HaHa') /// 替换字符串的第一个 !
let str5 = str.trim() //// 剔除字符串的首尾空格
let arr1 = str.split(',') /// 使用指定字符，分割原字符串，返回一个包含子串的数组

let str6 = str.substr(1, 3) /// 拷贝子串
let str7 = str.substring(1, 4) /// 拷贝子串
let str8 = str.slice(1, 4) /// 拷贝子串
```

* replace(old, new) 将字符串中的 old 子串替换为新的子串 new；
    * 注意：仅替换第一个出现的子串；
* trim(): 去除首尾空格，返回新的字符串；
* split(): 根据指定字符分割字符串；

拷贝子串：
* substr(startIndex, length)
* substring(startIndex, endIndex)
* slice(startIndex, endIndex)

### 4.2.2、数字常用方法

```
Math.PI 
Math.random()   /// 获取一个范围在 [0, 1) 的随机小数
Math.round(3.5) /// 对 3.5 四舍五入取整
Math.ceil(3.3)  /// 对 3.3 向上取整
Math.floor(3.5) /// 对 3.5 向下取整
Math.pow(2, 8)  /// 幂运算：2 的 8 次方
Math.sqrt(8)    /// 算术平方根
Math.abs(-8)    /// 绝对值
Math.max(-8, -7, 7, 8) /// 获取若干数字的最大值
Math.min(-8, -7, 7, 8) /// 获取若干数字的最小值
```

### 4.2.3、字典`Object`常用方法

```
/// 初始化
let dict = {'key1':'value1', 'key2':'value2'}
/// 增
dict.key3 = 'value3'
/// 删
delete dict.key1
```

### 4.2.4、数组常用方法

```
let arr = [1, '2', true] /// 创建数组
arr.length = 2 /// 设置数组长度，会将多余元素删除
arr.length /// 获取数组长度
arr.unshift('0') /// 数组头部增加一个新元素
arr.push('3') /// 数组末尾增加一个新元素
arr.shift() /// 删除数组最前面一个元素，并返回删除的元素
arr.pop() /// 删除数组最后一个元素，并返回删除的元素
arr.reverse() /// 反转数组
arr.splice(1, 2, 2, 3, 4) /// 删除与插入
arr.sort()  /// 默认从小到大排序
arr.sort(function(a, b) {return b - a}) /// 从大到小排序
let res = arr.join('+') /// 将数组中的每一项，使用指定连接符组成一个字符串
let res2 = arr.concat([6, 7, 8, 9]) /// 数组拼接
let res3 = res2.slice(5, 9) /// 拷贝数组的某些数据
let index = res.indexOf(9) /// 获取元素的索引

arr.forEach(function(item, index, arr) { /// 数组遍历
    confirm.log(item)
})
let arr2 = arr.map(function(item, index, arr) { /// 映射并返回新的数组
    return item + '1'
})
let arr3 = arr.filter(function(item, index, arr) { /// 过滤数组
    return item < 3
})
let res5 = arr.every(function(item, index, arr) { /// 判断数组的每一项都满足条件
    return item > 0
})
let res6 = arr.some(function(item, index, arr) { /// 判断数组是否有某些项满足条件
    return item > 3
})
```


* splice(startIndex, length, insert...) 函数：删除数组中若干数据，并旋转是否插入新的元素
    * startIndex 开始索引，默认为 0
    * length 长度， 默认为 0
    * insert 插入的数据，默认没有；如果插入多个数据，以 `,` 分割；从 startIndex 位置开始插入；
    * 返回值： 被删除的数据集；
* join('') 函数：数组中的每一项，使用指定连接符组成一个字符串，并返回字符串
* contact() 函数：将两个数组拼接，并返回新的数组
* slice(startIndex, endIndex): 拷贝数组的某些数据，对原数组无影响
    * startIndex 默认为 0
    * endIndex 默认为数组长度
    * 返回拷贝的数据集
* indexOf() ：获取指定元素出现在数组的第一个位置; 如果没有，则返回 -1；
* forEach() ：遍历数组，没有返回值
* map(): 映射数组，返回映射后的新数组；
* filter(): 过滤数组，并返回新的数组
* every() ： 判断数组的每一项是否满足条件，返回一个布尔值；
* some() ： 判断数组是否有某些项满足条件，返回一个布尔值；


### 4.2.5、时间 `Date` 常用方法

```
let date1 = new Date() /// Tue Nov 22 2012 11:15:15 GMT+0800 (中国标准时间)
let date2 = new Date(2002, 1, 15, 13, 38, 56) /// Fri Feb 15 2002 13:38:56 GMT+0800 (中国标准时间)
```

获取时间信息：

```
date1.getFullYear() /// 年
date1.getMonth()    /// 月
date1.getDate()     /// 日
date1.getHours()    /// 时
date1.getMinutes()  /// 分
date1.getSeconds()  /// 秒
date1.getDay()      /// 星期几 0～6: 周日～周六
date1.getTime()     /// 时间戳：以毫秒为单位
```

设置时间信息：设置年、月、日、时、分、秒、时间戳；不能设置周几！


# 5、运算符

JS 的运算符大致分为以下几类：
* 算数运算符：`+`、 `-`、 `*`、 `/`、 `%`（取余运算）
    * `+`符号两边都是数字，进行数学运算；
    * `+`符号任意一遍是字符串，进行字符串拼接；
* 赋值运算符：`=`、 `+=`、 `-=`、 `*=`、 `/=`、 `%=`；
* 比较运算符：比较的结果是一个布尔值；`>`、 `<`、 `>=`、 `<=`；
    * `==` 等于 与 `!=`不等于：只比较值是否相等，不考虑数据类型；
    * `===`全等于：必须值相等且数据类型相等；
    * `!==`不全等于：只要值或者数据类型，有一个不相等，就是不等；
* 逻辑运算符：`&&` 与、 `||` 或、 `!` 非；
* 自增自减运算符

```
console.log( 10 == '10')  /// true
console.log( 10 === '10') /// false

console.log( 10 != '10')  /// false
console.log( 10 !== '10') /// true
console.log( 10 !== 9)    /// true
```


# 6、定时器

* 间隔定时器：每隔一段时间执行一次；
* 延时定时器：在固定时间后执行一次；

```
let t1 = setInterval(function(){
    /// 每秒执行一次
}, 1000)

let t2 = setTimeout(() => {
    /// 延时 5 秒执行

}, 5000);

/// 返回值：返回该页面的第几个定时器

clearInterval(t1)
clearTimeout(t2)  /// 在延时定时器执行之前关闭
/// 注意：关闭定时器的函数不区分定时器种类
///      也就是说 clearInterval() 还可以关闭延时定时器；clearTimeout() 还可以关闭间隔定时器；
```

# 8、事件

## 8.1、事件绑定

事件绑定： `事件源.on事件类型=事件处理函数`
* 事件源：和谁 做好约定
* 事件类型 ：约定一个什么 行为
* 事件处理函数 ：当用户触发该行为的时候，执行什么代码

```
<body>
    <div></div>
    <script>
        /// 给 div 标签绑定一个点击事件
        let box = document.querySelector('div')
        box.onclick = function() {
            console.log('hahdfv ')
        }
    </script>    
</body>
```

## 8.2、事件类型

事件类型：
* 鼠标事件
* 键盘事件
* 浏览器事件
* 触摸事件
* 表单事件

鼠标事件|键盘事件|浏览器事件|触摸事件|表单事件
-|-|-|-|-
click 鼠标单击 | keydown 键盘按下 | load  加载完毕 | touchstart 触摸开始 | focus 聚焦
dblclick 鼠标双击 | keyup 键盘抬起 | scroll 滚动 | touchmove 触摸移动 | blue 失焦
contextmenu 左键单击 | keypress 键盘键入 | resize 尺寸改变 | touchend 触摸结束 | change 改变
mousedown 鼠标按下 | ... | ... | ... | input 输入
mouseup 鼠标抬起 | ... | ... | ... | submit 提交
mousemove 鼠标移动 | ... | ... | ... | reset 重置
mouseover 鼠标移入 | ... | ... | ... | ...
mouseout 鼠标移出 | ... | ... | ... | ...
mouseenter 鼠标移入 | ... | ... | ... | ...
moouseleave 鼠标移出 | ... | ... | ... | ...

## 8.3、事件对象

事件对象：当事件触发的时候，一个描述该事件信息的对象数据类型！

```
box.onclick = function(event) {
    /// event 事件对象
    /**
    {
        target : div.box
        type : "click"

        /// 鼠标事件·坐标信息
        clientX : 524   /// 相对于浏览器窗口的坐标
        clientY : 48

        offsetX : 514  /// 在响应盒子中的坐标
        offsetY : 39

        pageX : 524 /// 相对于页面文档流的位置
        pageY : 48
    }
    */
}
```

## 8.4、事件传播

浏览器响应事件的机制！
* 浏览器窗口最先知道事件的发生
* 捕获阶段：从 window 按照结构子级的顺序传递到目标;
* 目标阶段：准确触发事件的元素接收到行为;
* 冒泡阶段：从目标按照结构父级的顺序传递到 window
* 本次事件传播结束;

```
 /// 阻断响应链向父元素传递
event.stopPropagation();
```

## 8.5、事件委托

利用事件冒泡的机制，把自己的事件委托给结构父级中的某一层！

```
box1.onclick = function(event) {
    event.target.className /// 获取响应的子节点
}
```

# 9、ES 6

## 9.1、定义变量

* ES6 之前使用 var 定义变量
* ES6 之后新增 let 定义变量；const 定义常量；
    * var 可以进行预解析；而 let 与 const 不会预解析；
        * 预解析：在声明之前使用；
    * var 可以声明两个重名变量；而 let 与 const 不能定义重名变量；
    * var 没有块级作用域；而 let 与 const 有块级作用域；
* let 定义变量可以不赋初值，const 必须赋初值；
* let 定义变量可以被修改，const 不能修改；

## 9.2、箭头函数

箭头函数是 ES6 语法对函数表达式的简写；
* 内部没有 this ；

```
var f1 = function() {console.log('f1')}

var f2 = () => {console.log('f2')}

/// 省略 ()
var f3 = food => {console.log('f3', food)}
/// 只有一句代码，省略 {}
var f4 = (a, b) => a + b
```

箭头函数在某些时候可以省略 `()`
* 只有一个形参时；

箭头函数某些时候可以省略 `{}`, 并将该句代码的的执行结果当作函数的返回值
* 当只有一句代码时；

## 9.3、解构赋值

解构赋值：快速从对象或者数据中获取成员；
    * 数据的解构赋值；
    * 对象的解构赋值；

```
/// 快速从数组中获取值
let [a, b] = ['Hello', 'Word']

// 对象的解构赋值
let obj = {name:'张三', age: 18}
let {name} = obj
```

## 9.4、模板字符串

* 可以换行
* 可以在字符串内解析变量

```
let str = `Hello ${name}`
```

## 9.5、展开运算符

* 展开运算符 `...`
* 展开数组或者展开对象
* 作用：
    * 合并数组或者对象
    * 给函数传参数

```
let array = ['Hello', 'Word'
let obj = {name:'张三', age: 18}
```

## 9.6、类语法

必须和 new 一起使用，否则报错；

```
class Person {
    constructor(name) { /// 构造函数
        this.name = name
    }
    eat(food) {
        console.log('eat', food)
    }

    /// 使用 static 声明类属性与类方法
    static cName = '人类'
}
```

# 10、网络请求

```
/// json-server --watch demo.json --port 3100
var xhr = new XMLHttpRequest()
xhr.open('GET', 'http://localhost:3100/data', true)
xhr.onload = function() {
    console.log('请求完成', JSON.parse(xhr.responseText))
}
xhr.send()
```

# 11、jquery

## 11.1、jquery 操作元素

### 11.1.1、获取文本

```
// 等价于 innerHTML
$('div').html()                 /// get
$('div').html('<a>你好</a>')     /// set

// 等价于 innerText
$('div').text()
$('div').text('你好呀')

// 获取表单的 value 值
$('input').val()
$('input').val('你好呀')
```

### 11.1.2、操作类名

```
// 为 div 标签添加一个 类
$('div').addClass('box1')
// 自身没有则添加、自身有则删除再添加； 
$('div').toggleClass('box2')
// 删除
$('div').removeClass('box1')
```

### 11.1.3、jquery 操作样式

```
// get 样式
$('div').css('width')  
$('div').css('background-color') 

// set 样式
$('div').css('width', '300px')
$('div').css('height', '300px')
$('div').css('background-color', 'red')
$('div').css({
    'width': '200px',
    'height': '200px',
    'background-color': 'blue',
})
```

### 11.1.4、jquery 操作属性

```
// get
$('div').attr('id')   // 原生属性
$('div').attr('info') // 自定义属性

// set
$('div').attr('info', {name:'李四',age:18})

// remove
$('div').removeAttr('info')
```

`prop()` 获取和设置元素属性
* 当 `prop()` 设置元素的原生属性时，会直接响应到元素标签身上；
* 当 `prop()` 设置元素自定义属性时，不会响应到元素标签身上，响应到元素对应的对象身上；
* `prop()` 不能获取元素标签身上的自定义属性，只能获取到 `prop()` 方法自己设置的自定义属性；

```
/// 原生属性：设置到元素标签上
$('div').prop('class', 'box1')
/// 自定义属性：设置到对象上
$('div').prop('info', {name:'李四',age:18})

// 获取原生属性
$('div').prop('class')
// 获取对象身上的自定义属性
$('div').prop('info')
// 删除 prop() 函数设置的自定义属性；不能删除原生属性
$('div').removeProp('info')
```

### 11.1.5、jquery 获取元素尺寸

* 即使隐藏 display:none 也可以拿到尺寸
* 不论是盒模型或者怪异盒模型 `box-sizing: border-box;`，拿到的都是对应区域的尺寸；

```
// 纯内容区域:
$('div').width()
$('div').height()

// 内容区域 + padding 区域
$('div').innerWidth()
$('div').innerHeight()

// 内容区域 + padding 区域 + 边框区域
$('div').outerWidth()
$('div').outerHeight()
```

### 11.1.6、jquery 获取元素偏移量

```
/// 相对页面左上角 {top: 20, left: 20}
$('div').offset()
$('p').offset()
$('span').offset()

/// 元素的定位位置
$('div').position()
$('p').position()
$('span').position()
```

## 11.2、jquery 事件

### 11.2.1、jquery 事件绑定


* `on()` 绑定事件

```
// 为 box1 绑定点击事件
$('.box1').on('click', function(){
    console.log('box1 click')
})

// p1 将响应事件委托给 box1 处理
$('.box1').on('click', '.p1', function(){
    console.log('box1 click')
})

// 为 box1 批量绑定事件
$('.box1').on({
    'click': function(){console.log('box1 click')},
    'mouseover': function(){console.log('box1 mouse in')},
    'mouseout': function(){console.log('box1 mouse out')},
})
```

* `one()` 绑定事件: 仅执行一次；

```
// 为 box1 绑定点击事件
$('.box1').one('click', function(){
    console.log('box1 click')
})

// p1 将响应事件委托给 box1 处理
$('.box1').one('click', '.p1', function(){
    console.log('box1 click')
})

// 为 box1 批量绑定事件
$('.box1').one({
    'click': function(){console.log('box1 click')},
    'mouseover': function(){console.log('box1 mouse in')},
    'mouseout': function(){console.log('box1 mouse out')},
})
```

* `hover()` 事件

```
$('.box1').hover( /// 两个函数
    function(){console.log('移入')},    /// 移入触发
    function(){console.log('移出')}     /// 移出触发
)

$('.box1').hover(  /// 一个函数
    function(){console.log('移入 移出')},    /// 移入、移出触发
)
```

* 常用事件函数

```
$('.box1').click()
$('.box1').mouseover()
$('.box1').mouseout()
...
```


### 11.2.2、jquery 事件解绑

```
function f3() {console.log('clicl3')}
$('.box1').click(()=>{console.log('clicl1')})
            .click(()=>{console.log('clicl2')})
            .click(f3)

// 移出 f3 函数
$('.box1').off('click', f3)
/// 移出所有 click 事件
$('.box1').off('click')
```

### 11.2.3、jquery 事件触发

```
/// 代码触发事件
$('.box1').trigger('click')
```

## 11.3、jquery 动画函数

### 11.3.1、显示与隐藏

```
/// show() 动画
$('.box1').show(1000, 'linear', function(){
    console.log('show 动画结束')
})

/// hide() 动画
$('.box1').hide(1000)

/// show 与 hide 切换
$('.box1').toggle(100)
```

### 11.3.2、折叠与展开动画

* 本质就是改变盒子的高度

```
/// 折叠
$('.box1').slideDown(1000, 'linear', function(){
    console.log('展开 动画结束')
})

/// 展开
$('.box1').slideUp(1000)

// 折叠与展开切换
$('.box1').slideToggle(1000)
```

### 11.3.3、渐隐渐现动画

* 本质改变盒子的透明度

```
/// 渐出动画
$('.box1').fadeIn(1000, 'linear', function(){
    console.log('渐出 动画结束')
})

/// 渐隐动画
$('.box1').fadeOut(1000)

/// 渐隐于渐出动画切换
$('.box1').fadeToggle(1000)

/// 渐变到指定透明度
$('.box1').fadeTo(1000, 0.5, 'linear')
```

### 11.3.4、综合动画

`animate(param1, param2, param3, param4)` 参数:
* param1: 要运动的样式，以一个对象类型传递；
* param2: 运动时间
* param3: 时间曲线
* param4: 动画完成回调

注意：`transform` 动画、`color` 动画不支持；

```
$('.box1').animate(
    {
        width: '300px',
        height: '300px',
        opacity: '0.5',
    }, 
    1000, 
    'linear', 
    function(){
        console.log('动画结束') 
    }
)
```

### 11.3.5、结束动画

```
/// 立即停止：将动画停止到当前帧
$('.box1').stop()

/// 立即停止：将动画停止到最终帧
$('.box1').finish()
```


## 11.4、jquery 请求

```
function register(info) {
    $.ajax({
        url: 'http://localhost:3100/users',
        method: 'POST',
        async: true,
        data:info,
        success: function(response){
            console.log('success', response)
        },
        error: function(error) {
            console.log('error', error)
        }
    })
}
register({name:'张三', age: 18, sex: false})
```