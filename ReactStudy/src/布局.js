// 获取输入框的值 / 使用状态
import React, {Component} from 'react'

export default class InputDemo extends Component {

  constructor(){
    super()
    this.state = {
      btnstatus: false,
      list:[]
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
        <button onClick={ ()=>this.addClick()}>{"添加元素" + this.state.list.length}</button>
        
        {this.state.list.length>0 && /// 条件渲染
            <ul>{
              this.state.list.map((item, index)=>
                <li key={item.id}>
                  {item.text}
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
    if(this.myref.current.value.length) {
      /// 不建议直接修改状态值，可能造成未知的问题
      /// 深拷贝为一个新数组
      let newlist = [...this.state.list]
      newlist.push({
        id: this.myref.current.value + this.state.list.length,
        text:this.myref.current.value
      })
      this.setState({list: newlist})

      /// 清空输入框
      this.myref.current.value = ''
    }
  }

  deleteClick =(index)=> {
    let newlist = this.state.list.concat()
    newlist.splice(index, 1)
    this.setState({list: newlist})
  }
}


/** 
 * 
 * 
*/

