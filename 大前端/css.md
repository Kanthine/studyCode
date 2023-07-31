
CSS3是CSS技术的升级版本，CSS3语言开发是朝着横块化发展的。以前的规范作为一个横块实在是太庞大而且比较复杂，所以，把它分解为一些小的横块，更多新的横块也被加入进来。这些横块包括： 盒子横型、列表模块、超链接方式、语言横块、背最和边框、文字特效、多栏布局等。

CSS3的优点：CSS3将完全向后兼容，所以没有必要修改现在的设计来让它们继续运作。网络浏览器也还格继续支持CSS2。对我们来说，CSS3主要的影响是将可以使用新的可用的选择器和厲性，这些会允许实现新的设计效果(警如动态和渐变），而且可以很简单的设计出现在的设计效果（比如说使用分栏）。

* 渐进增强(progressive enhancement): 针对低版本浏览器进行构建页面，保证最基本的功能，然后再针对高级浏览器进行效果、交互等改进和追加功能达到重好的用户体验。
* 优雅降级(graceful degradation): 一开始就构建完整的功能，然后再针对低版本浏览器进行兼容。
* 区别：优雅降级是从复杂的现状开始，并试图减少用户体验的供给，而渐进增湿则是从一个非常基础的，能够起作用的版本开始，并不断扩充，以适应未来环境的需要。降级（功能衰減）意味者往回看；而渐进增强则意味着着朝前看，同时保证其根基处于安全地带。

# 1、CSS 属性与样式表

## 1.1、概念

* 每个 css 样式由两部分组成，即选择符与声明，声明又分为属性与属性值；
* 属性必须放在花括号中，属于与属性值用冒号连接；
* 每条声明使用分号结束；
* 当一个属性有多个属性值的时候，属性值与属性值无先后顺序，使用空格分隔；
* 在书写样式过程中，空格、换行等操作


外部样式表、内部样式表、行内样式表，生效原则一般而言是就近原则！如果有 `!important` 修饰，则最高优先级

优先级针对同一个标签同一个属性才有意义！

## 1.2、内部样式表

内部样式表，一般放在 `header` 部分！

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VSCode</title>
    <style>
        h1{
            color:red;
        }
        h2{
            color:blue;
        }
    </style>
</head>
<body>
    <h1>一级标题</h1>
    <h2>二级标题</h2>
</body>
</html>
```


## 1.3、行内样式表

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VSCode</title>
</head>
<body>
    <h1 style="color: red;">一级标题</h1>
    <h2 style="color: blue;">二级标题</h2>
</body>
</html>
```

## 1.4、外部样式表

.css 文件

```
h1{
    color: red !important;
}
h2{
    color:blue;
}
```

.html 文件

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VSCode</title>
    
    <!-- 引入 css 文件 -->
    
    <link rel="stylesheet" type="text/css" href="./demo.css"> 
    <style>
        @import url('./demo.css')
    </style>
    
</head>
<body>
    <h1>一级标题</h1>
    <h2>二级标题</h2>
</body>
</html>
```

link 与 import 区别：
* 本质区别，link 属于 HTML 的标签，import 完全是 css 提供的一种方式； 
* 加载顺序的差别，当一个页面被加载的时候，link 引用的 css 会同步被加载；import 引用的 css 会等到页面全部被下载完再被加载；所以有时候浏览 import 加载的 css 页面开始时没有样式；
* 兼容性：import 是 css2.1 提出的，老版本不支持；
* 建议使用 link 标签引用 css；
 

# 2、CSS 文本属性

属性|描述|说明
-|-|-
`font-size`|字体大小|单位是px，浏览器默认是 16px，设计常用字号是 12px
`font-family`|字体|当字体是中文字体、英文字体中有空格时，需要加双引号；多个字体中间用逗号连接，先解析第 1 个字体，如果没有再解析第 2 个字体，以此类推；
`color`|字体颜色| `color:red`，`color:#ff0`，`color:rgb(255, 255, 255)`
`font-weight`|加粗|`bolder` 更粗的； `bold` 加粗；`normal` 常规；`font-weight: 100～900`， 其中 `100~500` 不加粗、 `500~900` 加粗；
`font-style`|倾斜| `itlic` 斜字体；`oblique` 倾斜文字；`normal` 常规显示；
`text-align`|文本水平对齐| `left` 水平靠左、`right` 水平靠右、`center` 水平居中；`justity` 两端对齐，针对多行
`line-height`|行高| 
`text-indent` | 首行缩进 | 可以设置负值
`letter-spacing` | 间距 | 控制文件之间的间距：适用于中文字符。如果想设置英文单词间距，使用 `word-spacing`
`text-decoration` | 横线 | `underline` 下划线；`line-through` 删除线; `overline` 上划线

```
div{
    font-size: 20px;
    font-family: 微软雅黑, 宋体;
    font-weight: bold;
    text-align: justity;
    line-height: 30px;
    letter-spacing: 30px; /* 控制文件之间的间距：适用于中文字符 */
    word-spacing: 50px; /* 英文单词的间距 */
    text-indent: 5em; /* 首行缩进 */
    text-decoration: underline overline line-through; /* underline 下划线；line-through 删除线; overline 上划线*/
    color: #FF0000; /* red | #FF0000 | rgb(255, 0, 0) */
}
```

# 3、CSS 列表属性

属性|描述|说明
-|-|-
`list-style-type` | 定义列表符合样式 | `disc` 实心圆；`circle` 空心圆；`square` 实心方块；`none` 去掉方块；
`list-style-image` | 将图片设置为列表符合样式 | 
`list-style-position` | 设置列表项标记的放置位置 | `outside` 列表外部；`inside` 列表内部；
`list-style` | 简写 | `none` 去除列表符号

```
ul{
    border: 1px solid red;
    list-style-type: none;
    list-style-image: url(./1.png);
    list-style-position: inside;
    list-style: none url(./1.png) inside;
}
```

# 4、CSS 背景属性

属性|描述|说明
-|-|-
`background-color`| 背景色|
`background-image` | 背景图片 |
`background-repeat` | 背景图片的平铺 | `repeat`、 `no-repeat`、 `repeat-x`、 `repeat-y`；
`background-position` | 背景图片的定位 | 水平位置、垂直位置
`background-attachment` | 背景图片的固定 | `scroll` 滚动；`fixed` 固定于视口；
`background-size` ｜ 背景图片的 size | `cover` 将背景图片等比例扩展到足以覆盖容器；`contain` 背景图片等比例扩展到容器边缘，可能留白；

```
div{
    background-color: red;
    background-image: url(./1.png);
    background-repeat: no-repeat;
    background-size: 100% 100%;
    background-attachment: fixed;
    background-position: center center; /* 20px 50px */
    // 固定在视口的正中间
}
```

# 5、CSS 浮动属性

浮动：定义网页中的其它元素如何环绕该元素显示；

`float` 属性：
* `left`: 元素靠左边浮动；
* `right`: 元素靠右边浮动；
* `none`: 元素不浮动；


方案一：清浮动 `clear` 属性仅仅改变元素的排列方式，该元素还是浮动着；不占据文档位置；
* `left`: 不允许有左浮动
* `right`: 不允许有右浮动
* `both`: 不允许有浮动
* `none`: 允许有浮动对象

方案二：`overflow: hidden;`  让浮动元素计算 size;


# 6、盒子模型

盒子模型是 CSS 布局的基石，它规定了网页元素如何显示以及元素间相互关系。
CSS 定义所有的元素都可以拥有像盒子一样的外形和平面空间，即都包含边框、边界、补白、内容区，这就是盒子模型。

`margin`或者 `padding`或者`border`:
* 1个值：上下左右；
* 2个值：上下、左右；
* 3个值：上、左右、下；
* 4个值：上、右、下、左；


注意：
* `padding` 不支持负值；`margin` 支持负值；
* 不管内边距`padding`或者边宽`border`，都是向外扩展；即不挤占 `width` 与 `height` 的值；
* `margin` 的左右设置为 `auto` 可以显示水平居中效果；

特别问题：
* 1、兄弟关系，两个盒子垂直外边距与水平外边距的问题
    * 1.1、垂直外边距，两个盒子的 `margin-bottom` 与 `margin-top` 取最大值；
    * 1.2、水平外边距，两个盒子的 `margin-right` 与 `margin-left` 值相加、合并处理；
* 2、父子关系，子盒子加外边距，但作用于父盒子身上，怎么解决？
    * 2.1、父盒子加 padding 或者 父盒子设置 boder 都可以，但父盒子的实际宽高会向外扩展，需要注意减去扩展的宽高；
    * 2.2、使用浮动 `float`，使得父盒子与子盒子不在一个层面；
    * 2.3、`overflow: hidden` 实现；

# 7、溢出属性

`overflow`: `visible`、`hidden`、`scroll`、`auto`、`inherit`；
* `visible`: 默认值，溢出内容会显示在元素之外；
* `hidden`：溢出内容隐藏；
* `scroll`： 溢出内容以滚动方式显示;
* `auto`：如果有溢出会添加滚动条，没有溢出正常显示;
* `inherit`：规定应该遵从父元素继承overflow属性的值；
* `overflow-x`: X轴溢出； `overflow-y`: Y轴溢出；

## 7.1、空余空间

`white-space`: 用来设置如何处理元素内的空白`normal`、`nowrap`、`pre`、`pre-wrap`、`pre-line`、`inherit`、
* `normal`：默认值，空白会被浏览器忽略；
* `nowrap`: 文本不会换行，文本会在同一行上继续，直到遇到<br/>标签为止；
* `pre`: 类似于 `<pre>` 标签，能够保留空格、tab、回车, 常用于技术博客的代码段修饰；
* `pre-wrap`：换行；
* `pre-line`：只保留回车；

# 7.2、文本省略号

文本溢出之后以省略号显示，下述四个属性设置：

```
p {
    width: 100%; /* 宽度必须设置 */ 
    overflow: hidden;    /* 溢出内容隐藏 */
    white-space: nowrap; /* 文本不换行 */
    text-overflow: ellipsis; /* 文本溢出之后以省略号显示 */
}
```

# 8、元素显示类型

根据 CSS 的显示，可以将元素类型分为以下几类！

块元素: 如 `div`、`p`、 `ulli`、 `ollidl`、 `dt`、 `dd`、 `h1-h6`等；
* `display: block` 或者 `display: list-item`；
* 块状元素在网页中就是以块的形式显示，所谓块状就是元素显示为矩形区域
* 默认情况下，块状元素都会占据一行；块状元素会按顺序自上而下排列;
* 块状元素都可以定义自己的宽度和高度。
* 块状元素一般都作为其他元素的容器，它可以容纳其它内联元素和其它块状元素。我们可以把这种容器比喻为一个盒子;
* 盒子模型的所有元素在块元素中都生效；
* 注意：`p` 标签只能放文本，不能放其它块元素；

行内元素（内联元素）: 如 `a`、 `b`、 `em`、 `i`、 `span`、 `strong`等
* `display: inline`; 
* 内联元素的表现形式是始终以行内逐个进行显示;横着排
* 内联元素没有自己的形状，不能定义它的宽和高,它显示的宽度、高度只能根据所包含内容的高度和宽度来确定，它的最小内容单元也会呈现矩形形状;
* 内联元素也会遵循盒模型基本规则，但是对于 `margin` 和 `padding` 个别属性值不生效
* 如 `span` 、`b` 是有水平方向的间距生效、垂直方向的间距无法占据空间；

行内块元素：如 `img`、`input`等;
* `display: inline-block`; 
* 同时具备内联元素、块状元素的特点

## 8.1、元素类型转换

由于 `img` 标签属于行内块元素、`p` 标签属于块元素，不同类型的元素比较疏远，导致图片与文字之间有无法消除的间距！

```
    <img src="./img/3.png">
    <p>图片标题</p>
```

解决方案是将  `img` 标签改为块元素

```
img {
    display: block;
}
```


# 9、定位

定位 `position`:
* `position: static;` : 默认定位，按照文档流的排版顺序：从左到右、从上到下；
* `position: absolute;`：绝对定位；脱离了当前文档流，参照物“当没有父元素或者父元素没有定位，参照物是浏览器窗口的第一屏;有父元素且父元素有定位，父元素”！
* `position: relative;` 相对定位；还在当前文档流中，参照自己的原始位置做一定的偏移，但原来的位置空间还在；
* `position: fixed;` 固定定位，脱离当前文档流，参照物是浏览器的当前窗口；
* `position: sticky;` 粘性定位，可以做吸顶效果，粘性定位是css3.0新增加的，兼容不好

## 9.1、层级关系 `z-index`

注意：`z-index` 属性是不带单位的，并且可以给负值，没有设置z-index时，最后写的对象优先显示在上层，设置后，数值越大，层越靠上!
* 两个盒子都有定位且有相交部分时，默认后来者居上；
* 设置 z-index 越大，越靠近上层显示；设置的值越小，越靠近下层显示；
* 层级值 z-index 既可以设置正值、也可以设置负值；只关注最终值的大小；
* 如果没有设置定位，则 z-index 的设置毫无意义
* 父子关系时，将子盒子的 z-index 设置为负值，可以当父盒子在上层显示；

## 9.2、`absolute` 的额外用处

* 通过 `position: absolute;` 可以为一个行内元素设置宽高；

### 9.2.2、居中显示：

```
.box1 {
    width: 500px;
    height: 500px;
    background-color: red;
    /* 居中显示 */
    position: absolute;
    top: 50%;
    left: 50%;
    margin-top: -250px;
    margin-left: -250px;
}
```


### 9.2.3、绝对定位与浮动的区别

绝对定位与浮动，都是脱离文档流；后面的元素可以顶上去！但是后面的盒子有文字显示时：
* 浮动的盒子可以让文字围绕浮动盒子显示；浮动属于半脱离文档流；
* 绝对定位的盒子直接覆盖遮挡文字；


## 9.3、锚点

锚点作用：页面不同区域的跳转，使用 a 链接
* `<a href="锚点名称"></a>`
* `<div id="锚点名字"></div>`

```
<body>
    <ul>
        <li><a href="#语文">语文</a></li>
        <li><a href="#数学">数学</a></li>
        <li><a href="#英语">英语</a></li>
        <li><a href="#物理">物理</a></li>
        <li><a href="#化学">化学</a></li>
        <li><a href="#生物">生物</a></li>
    </ul>
    <div id="语文">语文</div>
    <div id="数学">数学</div>
    <div id="英语">英语</div>
    <div id="物理">物理</div>
    <div id="化学">化学</div>
    <div id="生物">生物</div>
</body>
```

## 9.4、精灵图

CSS Sprites的原理（图片整合技术）（CSS精灵)/雪碧图
* 将导航背景图片，按钮背景图片等有规则的合井成一张背景图，即将多张图片合为一张整图，然后用 `background-position` 来实现背景图片的定位技术。

图片整合的优势：
* 通过图片整合来减少对服务器的请求次数，从而提高面的加载速度；
* 通过整合图片来较少图片体积；


# 10、宽高自适应

自适应：网页布局中经常要定义元素的宽和高。但很多时候我们希望元素的大小能够根据窗口或子元素自动调整，这就是自适应！
* 宽度自适应：元素宽度的默认值 auto；
* 高度自适应：元素高度的默认值 auto；

## 10.1、浮动元素的高度自适应

问题：父元素不设置高廈，子元素写了浮动后，父元素会发生高廈場陷；
* 方法1：给父元素添加声明 `overflow:hidden`：（缺点：会隐藏溢出的元素）
* 方法2: 在浮动元素下方添加空块元素,井给该元素添加声明：`clear:both: height:0: overflow:hidden;` (缺点：在结构里增加了空的标签，不利于代码可读性，且降低了浏览器的性能）
* 万能清除浮动法: 选择符: `aftericontent:"";clear:both;display:block:height:0:visibility.hidden:;/overflow:hidden;`

```
<!-- 万能方法 -->
.box1::after {
    content: "";
    clear: both;
    display: block;
    width: 0px;
    height: 0px;
    visibility: hidden;  /* 占位隐藏 */
}
```

伪元素：
* `after` (content属性一起使用，定义在对象后的内容。
    * 如: `divafter/contenturllogojpg))`
    * 如 `div:afterfcontent:"文本内容°`
* `before`: 与content属性一起使用，定义在对象前的内容
* `first-letter` :定义对象内第一个字符的样式。
* `first-line`: 定义对象内第一行文本的样式


## 10.2、窗口高度自适应

```
html, body {
    height: 100%;
}
```


## 10.3、样式表计算

`calc()` 函数：用于动态计算长度值；
* 需要注意的是，运算符前后都需要保留一个空格；
* 任何长度值都可以使用该函数进行计算；
* 支持 `＋`，`-`，`*`，`/`运算；使用标准的数学运算优先级规则；

```
.box2 {
    width: calc(100% - 200px);
}
```