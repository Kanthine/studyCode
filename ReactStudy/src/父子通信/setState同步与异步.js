// setState 同步与异步的问题

import React, {Component} from 'react'
export default class StateAsync extends Component {

  constructor(){
    super()
    this.state = {
      count:1,
    }
  }

  // 创建一个引用，可以通过引用获取标签、获取组件
  myref = React.createRef()

  render(){
    let text = "测试状态"
    // 获取输入框的引用
    return (
      <div>
        <h1>{this.state.count}</h1>
        <button onClick={this.addClick}>加一操作</button>
        <button onClick={this.delayClick}>延迟加一</button>
      </div>
    )
  }

  addClick =()=>{

    /// 异步执行 setState，不会阻塞当前进程
    /// 等到一个事件循环结束之后，才会调用 setState 更新状态、刷新 ROM
    /// 连续调用多次 setState，并不会刷新多次 ROM；
    /// 多个 setState 会在React底层转换为一个，提高执行效率
    /// 多次合并之后，执行一次 this.state.count + 1
    this.setState({
      count:this.state.count + 1,
    })
    /// setState 结果尚未出来，就执行到此处
    console.log(this.state.count)

    this.setState({
      count:this.state.count + 1,
    })
    console.log(this.state.count)

    this.setState({
      count:this.state.count + 1,
    }, ()=>{
      /// 回调函数：状态和 ROM 更新完毕后被触发
      console.log(this.state.count)
    })

    /** React 代码中有一个标志位
     */
  }

  delayClick =()=>{
    setTimeout(()=>{

        /// 异步执行 setState，不会阻塞当前进程
        this.setState({
          count:this.state.count + 1,
        })
        /// setState 结果尚未出来，就执行到此处
        console.log(this.state.count)
        
        this.setState({
          count:this.state.count + 1,
        })
        console.log(this.state.count)

        this.setState({
          count:this.state.count + 1,
        })
        console.log(this.state.count)
    }, 1)
  }
}

/**
 * 在 ReactNative 中尽量减少 DOM 操作，因为 React 已经在做 DOM 操作了；
 * 我们只需告诉 React 我们想要什么，剩下的就是 React 自己去解决；所以，我们的组件需要使用 状态 这一个概念；
 *  
 * 状态：就是组件中描述某种显示情况的数据，由组件自己设置和更改，也就是由组件自己维护；
 *      使用状态的目的就是为了在不同的状态下使组件的显示不同（自己管理）
 *      在不操作 DOM 的情况下，只改变数据，让页面产生相应的改变；
 *      状态改变一次，DOM 会自动重新渲染
 * 
 * this.state 是纯 js 对象
 *      在 vue 中 data属性利用 Object.defineProperrty 处理过的，更改 data 数据的时候会触发数据的 getter 与 setter
 *      但是 React 没有这样的处理，如果直接更改，react 无法得知，所以需要使用 setstate 改变状态
 *      使用 setstate 一次可以修改多个状态
 * 
 * setstate 接收第二个参数：回调函数；状态和 ROM 更新完毕后被触发
 * setstate 处在同步的逻辑中，异步更新状态、更新真实 DOM
 * setstate 处在异步的逻辑中，同步更新状态、更新真实 DOM
 * 
 */