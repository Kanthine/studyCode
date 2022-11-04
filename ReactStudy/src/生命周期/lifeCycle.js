/// 生命周期
import React, {Component} from 'react'

export default class LifeDemo extends Component {

  state = {
    text:'',
  }

  

  UNSAFE_componentWillMount (){
    console.log('componentWillMount')
  }
  componentDidMount(){
    console.log('componentDidMount')
  }

  UNSAFE_componentWillUpdate(){
    console.log('componentWillUpdate')
  }
  componentDidUpdate(){
    console.log('componentDidUpdate')
  }

  shouldComponentUpdate(nextProps, nextState) {
    if(JSON.stringify(this.state) == JSON.stringify(nextState)) {
      return false
    }
    return true
  }

  static getDerivedStateFromProps(nextProps, nextState) {
    console.log('getDerivedStateFromProps',nextProps, nextState)
    return {text:'newText'}
  }

  render(){
    console.log('render')
    return (<div>
        <h1>生命周期</h1>
        <button onClick={()=>{

          /// 每次 setState ，事实上并没有改变 state
          /// 但 React 会把这个setState 做成新的虚拟 DOM，再与老的 DOM 对比
          /// 对比的过程中，可能生命周期仍在向前走、直到 render() 重新渲染后，才对比出 DOM 没有改变
          /// 白白浪费了大量资源、重复更新无必要的工作
          /// 可以使用 shouldComponentUpdate 判断是否重新渲染
          this.setState({text:'1'})
        }}>改变状态</button>
      </div>)
  } 

}

/** 生命周期：
 * 1、初始化阶段
 *  componentWillMount() : 
 *     已被废弃，不建议使用
 *        （在查找更新的状态时、很可能发生 render() 渲染的风险）
 *         (由于查找状态、相比于 render() 是低优先级，会被高优先级的任务打断执行)
 *        （此次没有完成状态更新、就会再次调用该方法，导致生命周期出现隐患）
 *     生命周期内仅执行一次
 *     render() 之前最后一次修改状态的机会
 *     用于初始化一些数据
 *  render() 只能访问 this.props 和 this.state，不允许修改状态和 DOM 输出
 *  componentDidMount() 
 *     生命周期内仅执行一次
 *     成功render()并渲染完成真实 DOM 之后触发，可以修改 DOM
 *     适合：网络数据请求、订阅函数的调用、事件的监听
 * 
 * 2、运行中阶段
 *  componentWillReceiveProps() 父组件修改属性触发子组件
 * 
 *  shouldComponentUpdate() 返回 false 会阻止 render 调用
 *  componentWillUpdate() 
 *      低优先级，可能被高优先任务打断，因此不安全；已被废弃；
 *      不能修改属性和状态，否则陷入死循环
 *  render() 只能访问 this.props 和 this.state，不允许修改状态和 DOM 输出
 *  componentDidUpdate() 可以修改 DOM
 *      更新后：可以获取 DOM 节点 
 * 
 * 3、销毁阶段
 *  componentWillUnmount() 在删除组件之前进行清理操作，比如计时器、事件监听器
 * 
 * 
 * 
 * 
 *** 新生命周期的替代
 *  getDerivedStateFromProps() 第一次的初始化组件以及后续更新过程中（包括自身状态更新及父传子），
 *                             返回一个对象作为新的 state，返回 NULL 则说明不需要在这里更新 state
 * 
 * 
 * 
 */