import { toHaveDisplayValue } from '@testing-library/jest-dom/dist/matchers'
import React, {Component} from 'react'


class NavBar extends Component {
  render() {
    return (<div style={{background:'red'}}>
      <button onClick={ ()=>{
        this.props.dismiss()
      }}>showAlert</button>
    </div>)
  }
}

class Alert extends Component {
  render() {
    console.log(this.props)
    return (<div style={{background:'yellow'}}>
      Alert
    </div>)
  }
}

export default class PCLink extends Component {

  constructor(){
    super()
    this.state = {
      showAlert:false,
      inputText:'',
      btnstatus: false,
      list:[]
    }
  }

  render(){
    let text = "测试状态"
    return (
      <div>
        <h1>欢迎使用状态</h1>
        <NavBar dismiss={()=>{
          this.setState({showAlert:!this.state.showAlert})
        }}/>

      {this.state.showAlert && <Alert/>}
 
        <input value={this.state.inputText} onInput={(event)=>{
          this.setState({inputText:event.target.value})
        }} />
        <button onClick={ ()=>this.addClick()}>{"添加元素" + this.state.list.length}</button>
        
        {this.state.list.length>0 && /// 条件渲染
            <ul>{
              this.state.list.map((item, index)=>
                <li key={item.id}>
                  <input type="checkbox" checked={item.selected} onChange={()=>{
                    let newlist = [...this.state.list]
                    newlist[index].selected = !newlist[index].selected;
                    this.setState({list: newlist})
                  }}/>
                  <span style={{textDecoration:item.selected?'line-through':''}}>{item.text}</span>
                  <button onClick ={()=>this.deleteClick(index)}>删除</button>
                </li>)       
            }</ul>
        }
        
        {this.state.list.length == 0 &&
            <h2>暂无数据</h2>
        }

        {/* //<h2 className={this.myref.current.value.length == 0 ? '': 'display:none'}>暂无数据</h2> */}

    </div>
    )
  }

  addClick =()=>{
    if(this.state.inputText.length) {
      /// 不建议直接修改状态值，可能造成未知的问题
      /// 深拷贝为一个新数组
      let newlist = [...this.state.list]
      newlist.push({
        id: this.state.inputText + this.state.list.length,
        text:this.state.inputText,
        selected:true
      })
      this.setState({list: newlist})

      /// 清空输入框
      this.state.inputText = ''
    }
  }

  deleteClick =(index)=> {
    
    let newlist = this.state.list.concat()
    newlist.splice(index, 1)
    this.setState({list: newlist})
  }
}

/**  组件通信方式
 * 
 * 父组件与子组件通信
 * 1、通信方式：
 *      父传子：传递数据
 *      子传父：传递方法
 * 2、ref 引用标记：父组件拿到子组件的引用，从而调用子组件的方法
 *    如父组件清除子组件 input 的输入文本：this.ref.form.reset()
 * 
 * 
 * 非父子组件通信方式
 * 1、状态提升（中间人模式）
 *    React 中的状态提升：概括来说，就是将多个组件需要共享的状态提升到它们最近的父组件上；
 *                     在父组件上改变这个状态然后通过 props 分发给子组件；
 * 2、发布订阅模式管理: 类似于监听模式
 * 
 * 3、context 状态树传参
 */