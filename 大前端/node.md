[Node 视频](https://www.bilibili.com/video/BV1rA4y1Z7fd/?spm_id_from=333.337.search-card.all.click&vd_source=3e79aa0ad6c6dd422df6eab7d2129711)

[Node 下载官网](https://nodejs.org/en/)


# 1、工具

## npm 使用

* `--global` 或 `-g` 下载到全局目录，默认安装到当前目录;
* `--dev` 开发用版本

```
MacBook-Pro $ node -v
v14.17.5
MacBook-Pro $ npm -v
6.14.14
MacBook-Pro $ npm install nodemon --global  /// --global 或 -g 下载到全局目录
MacBook-Pro $ npm uninstall nodemon --global  /// 卸载
MacBook-Pro $ npm install md5 --save --dev
MacBook-Pro $ npm i md5
MacBook-Pro $ npm i md5@2.3.0 /// 使用指定版本下载或覆盖
MacBook-Pro $ npm update md5
MacBook-Pro $ npm uninstall md5
MacBook-Pro $ npm list    /// 查看当前项目的下载包
MacBook-Pro $ npm list -g /// 查看全局下载包
MacBook-Pro $ npm info md5 /// 查看包信息
MacBook-Pro $ npm outdate /// 查看依赖库是否有过期的包
```

* `package.json` 文件记录依赖的第三方模块

```
/// 初始化一个 npm 环境、生成一个 package.json 文件
MacBook-Pro Demo $ npm init 

{
  "name": "test",
  "version": "1.0.0",
  "main": "index.js",
  "type": "module",  使用 ES6 引入模方式。默认是 commonjs 语法规范；两种规范不能混着用
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "dependencies": {
    "md5": "^2.3.0",  /// 锁死第一个大版本号，使用 npm install 下载时，会下载 2.* 的最新版本
    "md5": "~2.3.0",  /// 锁死前两个版本号， 会下载 2.3.* 的最新版本
    "md5": "2.3.0",   /// 锁死该版本
    "md5": "*",       /// 下载最新版本
  }
}
```

## nrm 使用

nrm 能快读切换镜像

```
MacBook-Pro $ npm install nrm -g /// 全局安装 nrm
MacBook-Pro $ nrm -V
1.2.5
MacBook-Pro $ nrm ls /// 查看镜像
  *npm ---------- https://registry.npmjs.org/
  yarn --------- https://registry.yarnpkg.com/
  tencent ------ https://mirrors.cloud.tencent.com/npm/
  cnpm --------- https://r.cnpmjs.org/
  taobao ------- https://registry.npmmirror.com/
  npmMirror ---- https://skimdb.npmjs.com/registry/

MacBook-Pro $ nrm use taobao /// 使用 taobao 镜像
MacBook-Pro $ npm config get registry /// 查看镜像
https://registry.npmmirror.com/
```

## yarn 

相比于 npm，
* 利用并行下载以最大化资源利用率，下载速度超快；
* yarn 可以缓存下载过的包，再次使用时无需重复下载；
* 超级安全，在执行代码执行，会利用算法校验每个安装包的完整性；

```
MacBook-Pro $ npm install yarn -g /// 全局下载 yarn
MacBook-Pro $ yarn init /// 初始化项目
MacBook-Pro $ yarn add md5 /// 添加包
MacBook-Pro $ yarn info md5 /// 查看包信息
MacBook-Pro $ yarn upgrade md5 /// 升级包
MacBook-Pro $ yarn remove md5 /// 移出包
MacBook-Pro $ yarn list /// 查看本地安装包列表
MacBook-Pro $ yarn install /// 下载所有依赖包
```

# 2、ES6 模块化写法

* `type: "module"`: 使用 ES6 引入模方式。默认是 commonjs 语法规范；两种规范不能混着用；

```
/// package.json 文件
{
  "name": "test",
  "version": "1.0.0",
  "main": "index.js",
  "type": "module",  使用 ES6 引入模方式。默认是 commonjs 语法规范；两种规范不能混着用
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  }
}
```

# 3、模块/包

基于 CommonJS 规范的 Node.js 模块：
* 内置的 Node.js 模块
* 第三方的 Node.js 模块
* 自定义的 Node.js 模块

## 3.1、常用的内置模块

内置模块：node 下载完毕后，已经存在于全局环境中的模块！

### 3.1.1、http 模块

### URL 模块

```
MacBook-Pro $ node
Welcome to Node.js v14.17.5.
Type ".help" for more information.
> url /// 查看 url 的方法
{
  Url: [Function: Url],
  parse: [Function: urlParse],
  resolve: [Function: urlResolve],
  resolveObject: [Function: urlResolveObject],
  format: [Function: urlFormat],
  URL: [class URL],
  URLSearchParams: [class URLSearchParams],
  domainToASCII: [Function: domainToASCII],
  domainToUnicode: [Function: domainToUnicode],
  pathToFileURL: [Function: pathToFileURL],
  fileURLToPath: [Function: fileURLToPath]
}
> url.parse('https://www.baidu.com:8080/s?wd=english') /// 解析 url 地址
Url {
  protocol: 'https:',
  slashes: true,
  auth: null,
  host: 'www.baidu.com:8080',
  port: '8080',
  hostname: 'www.baidu.com',
  hash: null,
  search: '?wd=english',
  query: 'wd=english',
  pathname: '/s',
  path: '/s?wd=english',
  href: 'https://www.baidu.com:8080/s?wd=english'
}
> 
```
