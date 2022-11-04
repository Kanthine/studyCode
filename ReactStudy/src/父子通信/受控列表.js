import React, {Component} from 'react'

export default class ControlledList extends Component {

  constructor(){
    super()
    this.state = {
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
 *      每执行一次 setstate，render() 方法就会被调用一次重新渲染
 * 
 * 非常不建议直接修改状态值
 *  this.state.list.push("aaa")
 */