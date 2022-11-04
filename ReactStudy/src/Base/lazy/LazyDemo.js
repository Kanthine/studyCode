import React, {Component, Suspense} from 'react'
const Child1 = React.lazy(()=>import('./Child1'))
const Child2 = React.lazy(()=>import('./Child2'))

export default class App extends Component {

  state = {
    showFirst: true,
  }

  render(){    
    let {showFirst} = this.state;

    return (
      <div>
        <h1>懒加载</h1>
        <button onClick={()=>{
          this.setState({showFirst: !showFirst})
        }}>{showFirst ? '去展示2' : '去展示1'}</button>

        <Suspense fallback={<div>正在加载 JS 文件...</div>}>
          { showFirst && <Child1/>}
          {!showFirst && <Child2/>}
        </Suspense>

    </div>
    )
  } 
}

/** 懒加载 ：React.lazy 函数能让你像渲染常规组件一样处理动态引入(的组件)。
 * 
 * 为什么代码要分割？
 *    当你的程序越来越大，代码量越来越多。一个页面上堆积了很多功能，也许有些功能很可能都用不到，
 *    但是一样下载加载到页面上，所以这里面肯定有优化空间。就如图片懒加载的理论。
 * 
 * 实现原理
 *    当 Webpack 解析该语法时，它会自动的开始进行代码分割 (Coding Splitting)，分割为一个个文件；
 *    当使用这个文件时这段代码才会被异步加载
 * 
 * 实现方案
 *    React.Lazy 和常用的第三方包 react-loadable 都是用上述原理，
 *    然后配合 Webpack 进行代码打包拆分达到异步加载，
 *    这样首屏渲染的速度大大的提高
 *    由于 React.Lazy 不支持服务端渲染，所以 react-loadable 是一个不错的选择
 */

