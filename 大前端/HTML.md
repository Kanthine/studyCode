[HTML 教学视屏](https://www.bilibili.com/video/BV1XF41187ZJ/?vd_source=3e79aa0ad6c6dd422df6eab7d2129711)

1、文本标题

<h1>一级标题</h1>
<h2>二级标题</h2>
<h3>三级标题</h3>
<h4>四级标题</h4>
<h5>五级标题</h5>
<h6>六级标题</h6>

注：文本标题标签自带加粗，有自己的文本大小、独占一行、有默认间距

2、段落文本

<p>段落文本内容</p>

3、换行

<br/>

4、水平线

<hr color="green" width="500" align="left" noshade/>

5、加粗标记 

<b>加粗</b>
<strong>加粗</strong> （推荐 strong）

6、倾斜

<em>倾斜</em>
<i>倾斜文本</i> （推荐 em）

7、删除线

<s>删除</s>
<del>删除文本</del> （推荐 del）

8、其它

<u>下划线</u>
<sub>下标</sub>
<sup>上标</sup>


9、特舒符号

特殊符号|解释
-|-
尖角号 | `&lt;` 左尖角号； `&gt;` 右尖角号
空格 | `&nbsp;` 占据宽度受字体影响
空格 | `&emsp;` 正好一个中文宽度，基本不受字体影响
版权 | `&copy;` 表示 ©
商标 | `&trade;` 表示 ™
商标 | `&reg;`表示 ®

10、div 与 span 标签

div 标签没有具体含义，用来划分页面区域、独占一行；


span 标签没有实际意义，不会破坏排版结构，主要应用于在对于文本独立修饰时，内容有多宽就占用多宽的空间距离；

```
<span style="color:blue">你好</span><span style="color:red">我好</span>
```

11、列表

11.1、无序列表

ul 标签的子标签只能是 li  标签；
type 取值: 
* `none`: 无效果；
* `disc`: 黑色实心圆； 
* `circle`: 空心圆；
* `square`: 黑色实心正方形

```
<ul type="square">
    <li>无序列表</li>
    <li>无序列表</li>
</ul>
```

11.2、有序列表

ol 标签的子标签只能是 li  标签； type 取值 `1`，`a`，`A`，`i`，`I`

```
<ol type="A" start="4">
    <li>有序列表</li>
    <li>有序列表</li>
</ol>
```

11.3、自定义列表

```
<dl>
    <dt>图片</dt>
    <dd>文字</dd>
</dl>
```

12、图片标签

src 图片路径：
* URL 地址；
* 同一个文件夹下，直接图片名称；
* 上一级文件夹下， '../demo.png'

title 鼠标悬停之后的提示信息
alt 图片加载失败的提示信息

```
<img title="哈咯啊" width="200" height="200" alt="加载失败，请再次刷新页面" src="../demo.png"/>
```

* 绝对路径：文件在硬盘正在存在的路径；一般很少使用；
* 相对路径：相对于自己的目标文件位置；'../demo.png'；

13、超链接标签

`href` 超链接
* URL 地址；
* 本地文件地址 `./child.html`；

`target` 在何处打开链接： 
* `_blank` 新的页面打开
* `_self` 默认值，原来页面打开
* `_parent` 
* `_top` 


```
<a href="https://www.baidu.com" title="标题" target="_blank">超链接文本</a>
```


14、数据表格的创建

table 表格属性：
* `border` 边框
* `bordercolor` 边框颜色
* `bgcolor` 背景色
* `width` 宽度
* `height` 高度
* `align` 对齐方式: left、right、center；
* `cellspacing` 单元格与单元格的间距
* `cellpadding` 单元格与内容的间距

行 tr 属性：
* `height` 高度
* `bgcolor` 背景色
* `align`  水平对齐方式: left、right、center；
* `valign` 垂直对齐方式: top、middle、bottom；

单元格 td 属性
* `bgcolor` 背景色
* `width` 宽度
* `height` 高度
* `align`  水平对齐方式: left、right、center；
* `valign` 垂直对齐方式: top、middle、bottom； 

```
<table border="1" bordercolor="red" bgcolor="gray" 
       width="50%" height="300" align="center" 
       cellspacing="10" cellpadding="50">  <!-- 创建表格 -->
    <tr height="160" bgcolor="red" valign="bottom"> <!-- tr 表示行  -->
        <td>第一行第一列</td> <!-- td 表示单元格  -->
        <td>第一行第二列</td>
    </tr>
    <tr>
        <td height="100" bgcolor="blue" align="right" valign="top">第二行第一列</td>
        <td>第二行第二列</td>
    </tr>
</table>
```

14.1、表格的合并

* `rowspan` 合并的行数
* `colspan` 合并的列数

```
<table border="1" width="50%" height="300">  
    <tr>
        <td width="25%">姓名</td>
        <td width="25%"></td>
        <td width="25%">性别</td>
        <td width="25%"></td>
    </tr>
    <tr> 
        <td>详细地址</td>
        <td colspan="3"></td>
    </tr>
    <tr> 
        <td>民族</td>
        <td></td>
        <td colspan="2" rowspan="2">照片</td>
    </tr>
    <tr> 
        <td>留言</td>
        <td></td>
    </tr>
</table>
```

15、表单

input 必须设置 name 属性，在提交表单时使用；另外 type 的值有：
* `text` ：普通文本；
* `password` ：密码；
* `button` ：普通按钮；
* `submit` ：提交；

```
<form mathod="get 或者 post" action="向何处发送表单数据，如：https://www.baidu.com/">
    用户信息：<input type="text" name="username" placeholder="请输入你的感受" > 
    <br/>
    用户密码：<input type="password" name="pwd" placeholder="请输入密码">
    <br/>
    <input type="submit" value="登陆">
    <br/>
    <input type="reset" value="重置">
</form>
```

## 15.1、表单类型

* `type='color'` 生成一个颜色选择的表单;
* `type='tel'` 唤起拨号盘表单;
* `type='searc'` 产生一个搜索意义的表单;
* `type='range'` 产生一个滑动条表单;
* `type='number'` 产生一个数值表单;
* `type='email'` 限制用户必须输入email类型;
* `type='url'` 限制用户必须输入url类型;
* `type='date'` 限制用户必须输入日期;
* `type='month'` 限制用户必须输入月类型;
* `type='week'` 限制用户必须输入周类型;
* `type='time'` 限制用户必须输入时间类型;
* `type='datetime-local'` 选取本地时间;


## 15.2、关联·选项列表

```
<input type="text" list="sList">
<datalist id="sList">
    <option value="上海">上海</option>
    <option value="上河">上河</option>
    <option value="北京">北京</option>
    <option value="北河">北河</option>
    <option value="广州">广州</option>
    <option value="广河">广河</option>
    <option value="深圳">深圳</option>
    <option value="深河">深河</option>
</datalist>
```

## 15.3、自动聚焦

`autofocus`属性：给文本框、选择框、或者按钮控件加上该属性，当打开页面时，该控件自动获得国标焦点，一个页面只能有一个!

## 15.4、

`required` 属性: 验证输入不能为空;

## 15.5、

`Multiple` 属性: 可以输入一个或多个值，每个值之间用逗号分开;


## 15.6、正则表达式`pattern` 

`pattern`: 将属性值设为某个格式的正则表达式，在提交时会检查其内容是否符合给定格式。


例：`<input pattern = “10-gJ/A-Z(3y” title二”输入内容：一个数与三个大写字母， placeholder=输入内容：一个数与三个大写宇母`


16、选择器

为什么使用选择器？要使用 CSS 对 HTML 页面中的元素实现一对一、一对多或者多对一的控制，需要用到 CSS 选择器！

CSS 选择器解析规则：
* 当多个选择器、选中同一个元素，且都定义了样式，如果属性发生冲突，则选择权重高的来执行；
* 相同权重的选择器，样式遵循就近原则；哪个选择器后定义，就采用哪个选择器的样式；
* 权重：!important > 内联样式 > 包含选择器(权重之和) > id 选择器 > 类别选择器 > 元素选择器!


选择器 | 权重
-|-
元素选择器 | 00001
类别选择器 | 00010
id 选择器 | 00100
包含选择器 | 包含的选择器权重之和
内联样式   | 01000
!important | 10000

注意：!important 不要频繁使用！

16.1、元素选择器

元素选择器以文档语言对象类型作为选择符，即使用结构中的元素作为选择符，如 `body`、`div`、`p`、`emg`、`span`等；所有的页面元素都可以作为选择符！

如果想改变某个元素默认样式、或者统一文档的某个元素的显示效果时，可以使用此类选择器。

```
标签名称{
    属性: 属性值;
}

div{
    background-color: red;
}
```

16.2、类别选择器

类别选择器更适合自定义某一个样式！

```
.名字{
    属性: 属性值;
}

.divB1{
    background-color: blue;
}

.divT1{
    color: red;
}
```

使用

```
<div class="divB1 divT1">你是二</div>
```

16.3、id 选择器

* 当使用 id 选择符时，应为每一个元素定义一个 id 属性
* id 选择符的语法是 `#id名称{属性: 属性值}`
* id名称 应使用英文名称，不能使用关键字（所有的标记与属性都是关键字）
* 不同于类别选择器可以使用多个选择器，一个 id 名称只能对应文档中的一个具体的元素对象（唯一性）；

css 文件中定义样式

```
#liu3 {
    background-color: black;
    color: white;
}

#liu4 {
    background-color: orange;
}
```

HTML 中使用

```
<div id="liu3">你是三</div>
<div id="liu4">你是四</div>
```

16.4、通配选择器

```
*{
    属性: 属性值;
}
```

常用于将所有元素的边距清零等操作：

```
*{
    margin: 0;
    padding: 0;
}
```

16.5、群组选择器

当有多个选择符应用相同的声明时，可以将选择符用 `,` 分割，合并到一组！

`margin: 0 auto` 水平居中

```
h1, h3, h5, .divT1{
    color: red;
}

h2, h4, h6{
    color:blue;
}
```

演示：
 
```
<h1>一级标题</h1>
<h2>二级标题</h2>
<h3>三级标题</h3>
<h4>四级标题</h4>
<h5>五级标题</h5>
<h6>六级标题</h6>
``` 

16.6、包含选择器或者后代迭代器

用法：当某个元素存在父级元素的时候，要改变自己本身的样式，可以不另加选择符，直接用包含选择器的方式解决。

CSS 定义样式：

```
语法: 选择符1 选择符2 {属性: 属性值} 
含义: 在选择符1 中包含的所有 选择符2

div h1{
    color: white;
    background-color: red;
}

div>h1{
    color: white;
    background-color: red;
}
```

选择符之间使用空格分割，表示后代选择器；使用 `>` 分割，表示儿子选择器；

HTML 中只有 div 包含的 h1 标签才能使用该样式

```
<h1>一级标题</h1>

<div>
    <h1>一级标题</h1>
</div>
```



16.8、属性选择器 


```
/* 凡是具有 class 属性的标签  */
[class] {
    color: white;
    background-color: red;
}

/* 凡是具有 name 属性的 input 标签 */
input[name] {
    border: 2px solid red;
}

/* div 标签中 class = box1 
 * 注意：此处是完全等于，如果标签的 calss 属性还有其它值，则不属于完全等于
 */
div[class=box1] {
    border: 1px solid white;
}

/* div 标签中 class ～= box2
 * 注意：此处 ～= 是包含，如果标签的 calss 属性还有其它值，也可以匹配到
 */
div[class~=box1] {
    border: 1px solid white;
}
```


模糊匹配：


```
<!-- 匹配以 b 开头的 class 属性 -->
class^=b {
    color: white;
    background-color: red;
}

<!-- 匹配以 b 结尾的 class 属性 -->
class$=b {
    color: white;
    background-color: red;
}

<!-- 匹配包含 b 的 class 属性 -->
class*=b {
    color: white;
    background-color: red;
}
```


16.9、伪类选择器 

16.9.1、结构伪类选择器 

* `X:first-child` 匹配子集的第一个元素；
* `X:last-child` 匹配父元素中最后一个X元素
* `X:nth-child(n)` 用于匹配索引值为n的子元素。索引值从1开始
* `X:only-child` 这个伪类一般用的比较少，比如上述代码匹配的是div下的有且仅有一个的p，也就是说，如果div内有多个p，将不匹配。
* `X:root` 匹配文档的根元素。在HTML (标准通用标记语言下的一个应用）中，根元素永远是HTML;
* `X:empty` 匹配没有任何子元素（包括包含文本）的元素x

```
.box1 div:last-child {
    margin-right: 0px;
}

/** .box2 下面 li 的偶数标签 
    * 偶数 even 或者 2n
    * 奇数 odd 或者 2n - 1
    * 3n：3、6、9 等是 3 的倍数的标签
*/
.box2 li:nth-child(3n) {
    background-color: red;
}

/* div 标签中只有一个 p 标签才生效 */
.box3 div p:only-child {
    background-color: blue;
    color: white;
}

/* div 标签中为空标签；
* 注意：空格、回车符也不算空标签 
*/
.box3 div:empty {
    width: 50px;
    height: 50px;
    background-color: yellow;
}

/* :root 根选择器  */
:root,body {
    height: 100%;
    background-color: green;
}
```

16.9.2、目标伪类选择器

E:target 选择匹配E的所有元素，且匹配元素被相关URL指向;


```
div:target {
    background-color: brown;
    display: block;
}
```

16.9.3、UI状态伪类选择器

`E:enabled` 匹配所有用户界面(form表单）中处于可用状态的E元素;
`E:disabled` 匹配所有用户界面 (form表单）中处于不可用状态的E元素;
`E:checked` 匹配所有用户界面 （form表单）中处于选中状态的元素E;
`E:selection` 匹配E元素中被用户选中或处于高亮状态的部分;

```
/* 非禁用状态 */
input:enabled {
    background-color: red;
}

/* 禁用状态 */
input:disabled {
    background-color: blue;
}

/* 处于焦点的 input 标签 */
input:focus {
    background-color: yellow;
}

input[type=checkbox] {
    /* 清除默认样式 */
    appearance: none;
    width: 20px;
    height: 20px;
    background-color: transparent;
    border: 2px solid black;
}

/* 勾选样式 */
input:checked {
    background-color: green;
}

/* p 标签选中部分的文字样式 */
p::selection {
    background-color: red;
    color: white;
}
```


16.9.4、否定伪类选择器

```
/* 不是第一个标签 */
li:not(:first-child) { 
    background-color: red;
}

li:not(:nth-child(3n)) {
    background-color: blue;
}
```

16.9.5、 动态·伪类选择器

语法：Link--visited--hover--active
* a:link{属性：属性值}超链接的初始状态;
* a:visited{属性：属性值} 超链接被访问后的状态;
* a:hover{属性：属性值} 鼠标悬停，即鼠标划过超链接时的状态;
* a:active{属性：属性值} 超链接被激活时的状态，即鼠标按下时超链接的状态;

说明：
* A）当这4个超链接伪类选择符联合使用时，应注意他们的顺序，正常顺序为：a:link,a:visited,a:hover,a:active,错误的顺序有时会使超链接的样式失效；
* B）为了简化代码，可以把伪类选择符中相同的声明提出来放在a选择符中；例如：a{color:red} a:hover{color:green} 表示超链接的初始和访问过后的状态一样，鼠标划过的状态和点击时的状态一样。

CSS 中定义样式

```
/* 初始状态 */
a:link {
    color: red;
    background-color: blue;
}

/* 访问之后 */
a:visited { 
    color: white;
    background-color: black;
}

/* 鼠标移上去之后 */
a:hover {
    color: blue;
    background-color: gray;
}

/* 鼠标点击的效果 */
a:active {
    color: black;
    background-color: yellow;
}
```

HTML 中使用样式

```
<a href="https://www.baidu.com/">超链接</a>
```


# 17、常见的布局方案

* 固定布局：以像素作为页面的基本单位，不管设备屏幕及浏览器宽度，只设计一套尺寸；
* 可切换的固定布局：同样以像素作为页面单位，参考主流设备尺寸，设计几套不同宽度的布局。通过识别的屏幕尺寸或浏览器宽度，选择最合适的那套宽度布局;
* 弹性布局：以百分比作为页面的基本单位，可以适应一定范围内所有尺寸的设备屏幕及浏览器宽度，并能完美利用有效空间展现最佳效果；
* 混合布局：同弹性布局类似，可以适应一定范国内所有尺寸的设备屏幕及浏览器宽度，并能完美利用有效空间展现最佳效果;只是混合像素、和百分比两种单位作为页面单位。
* 响应式布局：对页面进行响应式的设计实现，需要对相同内容进行不同宽度的布局设计，有两种方式：
    * PC优先（从pc端开始向下设计）；
    * 移动优先（从移动端向上设计）;
    * 无论基于那种横式的设计，要兼容所有设备，布局响应时不可避免地需要对模块布局做一些变化 （发生布局改变的临界点称之为断点）

## 17.1、媒体查询

媒体查询可以让我们根据设备显示器的特性（如视口宽度、屏幕比例、设备方向：横向或纵向）为其设定CSS样式，媒体查询由媒体类型和一个或多个检测媒体特性的条件表达式组成。媒体查询中可用于检测的媒体特性有 width、height 和color（等）。使用媒体查询，可以在不改变页面内容的情况下，为特定的一些输出设备定制显示效果。

### 媒体查询操作方式

实际操作为：对设备提出询问（称作表达式）开始，如果表达式结果为真，媒体查询中的CSS被应用，如果表达式结果为假，媒体查询内的CSS将被忽路。

```
@media all and (min-width:320px) {
    body { background-color:blue;)
}


<!-- 竖屏：宽度小于高度 -->
@media screen and (orientation portrait) and (max-width: 720px)

<!-- 横屏：宽度大于高度 -->
@media screen and (orientation:landscape)(对应样式）
```

## 17.2、响应式布局的特点

设计特点：
* 面对不同分辨率设备灵活性强；
* 能够快捷解决多设备显示适应问题；

缺点：
* 兼容各种设备工作量大，效率低下
* 代码累赘，会出现隐藏无用的元素，加载时间更长；
* 其实这是一种折中性质的设计解决方案，多方面因素影响而达不到最佳效果；
* 一定程度上改变了网站原有的布局结构，会出现用户混淆的情况；

## 17.3、单位

* `px`: 像素单位；
* `em`: 相对单位，相对于父元素的字体大小
* `rem`: 相对单位，相对于根元素的字体大小


# 18、渐变

## 18.1、线性渐变

```
background: linear-gradient(to top right, red, blue, yellow, black);

background: linear-gradient(110deg, red, blue, yellow, black);
```

## 18.2、径向渐变

径向渐变不同于线性渐变，线性渐变是从“一个方向”向“另一个方向”的颜色渐变，而径向渐变是从 “一个点〞向四周的颜色渐变！

```
background: radial-gradient(red 10%, blue 50%, yellow 70%);

/* shape：渐变的形状，elipse表示椭园形，circle表示圆形。默认为ellipse，如果元素形状为正方形的元素，则ellipse和circle显示一样。 */
background: radial-gradient(circle,red, blue, yellow);

/* farthest-side：最远边;closest-corner：最近角; farthest-corner :最远角 ; closest-side */
background: -webkit-radial-gradient(60% 40%, farthest-corner, red, blue, yellow);
```

## 18.3、重复渐变


```
background: -webkit-repeating-radial-gradient(red 10%, blue 20%);
```


# 19、过度

```
transition: all 2s linear 1s;
/* all 所有属性
    2s 动画持续时间 
    linear 线性动画
    ease 减速动画
    ease-in 加速
    ease-out 减速
    ease-in-out 先加速后减速
    1s 动画延迟执行时间
    除了 display：none 属性
    */

transition-property: width;
transition-duration: 2s;
transition-timing-function: cubic-bezier(0.075, 0.82, 0.165, 1);
transition-delay: 0s;
```

# 20、transform

设置 left 属性会频繁的造成浏览器回流重排，而 transform 和 opacity 属性不会，因为它是作为合成图层发送到 GPU 上，由显卡执行的渲染，这样做的优化如下:
* 可以通过亚像素精度得到一个运行在特殊优化过的单位图形任务上的平滑动画,并且运行非常快。
* 动画不再绑定到 CPU 重排，而是通过 GPU 合成图像。即使运行一个非常复杂的 JavaScript 任务，动画仍然会很快运行。

```
/// 平移
transform: translateX(300px) translateY(300px) translateZ(300px) scaleX(0.5) scaleY(0.1);

/// 缩放
transform: scaleX(0.5) scaleY(0.1);

/// 旋转
transform: rotate(100deg);

// 倾斜 ：能够让元素倾斜显示。它可以将一个对象以其中心位置围绕着X轴和Y轴按照一定的角度倾斜。
transform: skew(30deg, 150deg);
```
