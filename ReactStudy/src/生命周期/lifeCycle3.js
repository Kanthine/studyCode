/// 生命周期· componentWillReceiveProps() 函数
import React, {Component} from 'react'

class Box extends Component {

  shouldComponentUpdate(nextProps){
    let {currentItem, item} = this.props;

    if(currentItem==item || /// 更新上一次相等的
      nextProps.item==nextProps.currentItem) { /// 更新这一次相等的
      return true
    }
    return false
  }

  render() {
    let {currentItem, item} = this.props;
    return (<div style={{width:'200px', height:'30px', 
                 border:currentItem==item?'5px solid red' : '5px solid black', 
                 margin:'10px',float:'left'}}>{item}
    </div>)
  }
}

export default class LifeDemo3 extends Component {

  state = {
    list:[9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
    currentItem:8
  }

  myref = React.createRef()
  getSnapshotBeforeUpdate() {
    /// 获取容器高度
    console.log(this.myref.current.scrollHeight)
    return this.myref.current.scrollHeight
  }

  componentDidUpdate(prevProps, prevState, value) {
    this.myref.current.scrollTop += this.myref.current.scrollHeight - value
  }

  render(){
    return (<div>
        <h1>邮箱案例</h1>
        <button onClick={(event)=>{ 
            let newlist = this.state.list;
            this.setState({list: [newlist.length, ...newlist]}) 
          }}>添加内容</button>
        <div style={{height:'300px', overflow:'auto'}} ref={this.myref}>
          {
            this.state.list.map((item, index)=>
              <Box key={item} currentItem={this.state.currentItem} index={index} item={item} />)
          }
        </div>
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
 *  componentWillReceiveProps() 父组件修改属性触发
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
 */