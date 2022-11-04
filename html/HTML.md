
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

16.7、 伪类选择器

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