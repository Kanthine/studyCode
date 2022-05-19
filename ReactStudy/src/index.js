/// 只要使用 React.js 就必须引入 React，因为 react 包含 JSX
/// 从 react 的包当中引入了 React
import React from 'react'

// ReactDOM 可以帮助我们把 react 组件渲染到页面上去；它是从 `react-dom` 引入而非 `react`
import ReactDOM from 'react-dom'


/// 介绍组件化与样式
/// import APP from './mainApp'
/// ReactDOM.render(<APP/>, document.getElementById("root"))

/// 介绍按钮事件
// import ClickDemo from './btnClick'
// ReactDOM.render(<ClickDemo/>, document.getElementById("root"))

/// 介绍输入框
import InputDemo from './input'
ReactDOM.render(
  <React.StrictMode>
    <InputDemo/>
  </React.StrictMode>,
  document.getElementById("root"))

/// ReactDOM 里面有一个 render 方法：渲染组件并且构造 DOM 树，然后插到页面的某个特定元素上去
/// ReactDOM.render("Hello Demo", document.getElementById("root"))
// ReactDOM.render(<div>
//     <b>Hello Demo</b>
//     <ul>
//       <b>Hello Demo 1</b>
//       <b>Hello Demo 2</b>
//       <b>Hello Demo 3</b>
//       <b>Hello Demo 4</b>
//       <b>Hello Demo 5</b>
//       <b>Hello Demo 6</b>
//     </ul>
//   </div>, document.getElementById("root"))
/// 所谓 JSX 其实就是 JavaScript 对象，使用 React 和 JSX 的时候一定会经过编译的过程：
/// JSX -- 使用 react 构造组件，bable 进行编译 -> JavaScript 对象 -> ReactDOM.render(） DOM 元素 -> 插入页面
