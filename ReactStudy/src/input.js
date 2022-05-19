// 获取输入框的值
import React, {Component} from 'react'

export default class InputDemo extends Component {

  count = 100

  // 创建一个引用，可以通过引用获取标签、获取组件
  myref = React.createRef()

  render(){
    return (
      <div>
        /// 获取输入框的引用
        <input ref = {this.myref} />
        <button onClick={ ()=>{
            ///匿名函数
            console.log("匿名函数",this.count)
            console.log("匿名函数",this.myref.current.value)
        }}>获取输入框文本</button>
    </div>
    )
  }
}
