import React, {Component} from 'react'

export default class App extends Component {

  state={
    inputText:'',
    list:['1','2','3','4','5','6']
  }


  render(){    
    return (
      <div>
        <h1>单元测试</h1>
        <input onChange={(event)=>{
          this.setState({inputText: event.target.value});
        }}/>
        <button className='add' onClick={()=>{
          this.setState({list:[...this.state.list, this.state.inputText]});
        }}>添加</button>
        <ul>{
            this.state.list.map((item, index)=>
              <li key={index}>{item}<button className='del' onClick={()=>{
                  var newList = [...this.state.list]
                  newList.splice(index, 1)
                  this.setState({list: newList})
              }}>删除</button></li>
            )
        }</ul>
    </div>
    )
  } 
}

class Child extends Component {

  render(){
    return (
      <div>
        Child
        {this.props.children[1]}
        {this.props.children[0]}
    </div>)
  } 
}

/**
 * npm install react-test-renderer
 * 
 * 单元测试：每次版本迭代、快速的进行回归测试
 * 
 * 
*/