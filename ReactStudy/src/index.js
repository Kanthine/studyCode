/// 只要使用 React.js 就必须引入 React，因为 react 包含 JSX
/// 从 react 的包当中引入了 React
import React from 'react'

// ReactDOM 可以帮助我们把 react 组件渲染到页面上去；它是从 `react-dom` 引入而非 `react`
import ReactDOM from 'react-dom'


/// 介绍组件化与样式
// import APP from './mainApp'
// ReactDOM.render(<APP/>, document.getElementById("root"))

/// 介绍按钮事件
// import ClickDemo from './btnClick'
// ReactDOM.render(<ClickDemo/>, document.getElementById("root"))

/// 介绍输入框与状态
// ReactDOM.render(
//   <React.StrictMode>
//     <InputDemo/>
//   </React.StrictMode>,
//   document.getElementById("root"))

/// 介绍数组
// import ArrayDemo from './array'
// ReactDOM.render(<ArrayDemo/>, document.getElementById("root"))

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


/// React 18 新创建方式
import { createRoot } from 'react-dom/client';

// import APP_1 from './appDemo1';
// import APP_2 from './APP_2/appDemo2'
// import StateAsync from './setState同步与异步';
// import PrposDemo from './属性prpos';
// import ControlledDemo from './受控组件';
// import InputDemo from './input'
// import ControlledList from './受控列表';
// import PCLink from './父子通信';
// import Forms from './父子通信/表单域组件';
// import DispathMessage from './父子通信/订阅模式';
// import ContenxtDemo from './父子通信/context通信';
// import SlotDemo from './slot';
// import LifeDemo from './生命周期/lifeCycle';
// import LifeDemo2 from './生命周期/lifeCycle2';
// import LifeDemo3 from './生命周期/lifeCycle3';
// import PureEqual from './生命周期/pureEqual';
// import SwiperDemo from './swiperDemo';
// import HOOK1 from './hooks/hook1';
// import SheetDemo from './sheetDemo';
// import HOOK4 from './hooks/hook4';
// import RouterApp from './router/routerAPP';
// import StyleDemo1 from './页面布局/style1';
// import AppUI from './UI_PC/UIpc';
// import LayoutUI from './UI_PC/LayoutPC';
// import RouterApp from './ReactRedux/routerAPP';
// import PhoneUI from './UI_Phone/UIphone';
// import {store, persistor} from './ReactRedux/Redux/store';
// import { PersistGate } from 'redux-persist/integration/react';
import App from './后台管理系统/App';

// import RouterApp from './MobxDemo/routerAPP';
// import { Provider } from 'mobx-react';
// import store from './MobxDemo/Mobx/store';

const container = document.getElementById("root")
const root = createRoot(container)
root.render(<App/>)
// root.render(<PCLink/>)
// root.render(<ControlledList/>)
// root.render(<APP_1/>)
// root.render(<APP_2/>)
// root.render(<InputDemo/>)
// root.render(<StateAsync/>)
// root.render(<PrposDemo/>)
// root.render(<ControlledDemo/>)
// root.render(<Forms/>)
// root.render(<DispathMessage/>)
// root.render(<ContenxtDemo/>)
// root.render(<Provider store={store}><RouterApp/></Provider>)



// root.render(<Provider store={store}>
//     {/* 持久化网关 */}
//     <PersistGate loading={null} persistor={persistor}><RouterApp/></PersistGate> 
// </Provider>)


/** React 大纲：
 *  React 基础知识
 *  React Hooks 
 *  React 路由 
 *  Redux 
 *  组件库 
 *  Immutable 
 *  Mobx 
 *  React+TS 
 *  单元测试
 *  dva+umi 
*/