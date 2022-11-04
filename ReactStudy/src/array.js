// 获取输入框的值
import React, {Component} from 'react'

export default class ArrayDemo extends Component {

  constructor(){
    super()
    this.state = {
      btnstatus: false ,
      list:["item_1", "item_2", "item_3", "item_4", "item_5"]
    }
  }

  // 创建一个引用，可以通过引用获取标签、获取组件
  myref = React.createRef()

  render(){
    let text = "测试状态"
    // 获取输入框的引用
    return (
      <div>
        <h1>欢迎使用状态</h1>
        <input ref = {this.myref}></input>
        <button onClick={ ()=>{
          this.setState({btnstatus: !this.state.btnstatus}) /// 间接修改状态
            ///匿名函数
            console.log("匿名函数",this.myref.current.value)
        }}>{this.state.btnstatus ? "选中态" : "未点击"}</button>


        <ul>{
          this.state.list.map((item, index)=><li key={index}>{item}</li>)           
        }</ul>
    </div>
    )
  } 
}

/** 如无必要、勿增实体
 * map() 映射函数：将数组中每一个元素映射为另一个元素，并返回一个等长的新数组；
 * 
 * 将数组 list 中的每一个元素映射为 <li>{item}</li>，实现 DOM 创建工作
 * newlist = list.map(item=>`<li>${item}</li>`)
 * 在 React 中再次改进写法：
 * list.map(item=><li>{item}</li>)
 * 
 * 再次改进
 * 
 * 每一个 li 中必须有一个 key 属性：
 *    为什么要加 key ？为了列表的复用与重排
 * 
 * 
 * this.state 是纯 js 对象
 *      在 vue 中 data属性利用 Object.defineProperrty 处理过的，更改 data 数据的时候会触发数据的 getter 与 setter
 *      但是 React 没有这样的处理，如果直接更改，react 无法得知，所以需要使用 setstate 改变状态
 */