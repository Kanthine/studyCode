import React, {Component} from 'react'



class Filed extends Component {

  render() {
    return (<div style={{background:'red'}}>
      <label>{this.props.label}</label>
      <input type={this.props.text} value={this.props.value} onChange={(event)=>this.props.onChange(event)}></input>
    </div>)
  }
}

export default class Forms extends Component {
  
  state={
    userName: localStorage.getItem('userName'),
    password: localStorage.getItem('password'),
  }

  render(){
    let text = "测试状态"
    return (
      <div>
        <h1>表单域组件通信</h1>
        <Filed label="用户名" text='text' value={this.state.userName} onChange={(event)=>{
          this.setState({userName:event.target.value})
        }}/>
        <Filed label="密码" text='password' value={this.state.password} onChange={(event)=>{
          this.setState({password:event.target.value})
        }}/>
        <button onClick={()=>{
          console.log(this.state.userName, this.state.password)
          localStorage.setItem('userName',this.state.userName)
        }}>登陆</button>
        <button onClick={()=>{
          
        }}>取消</button>
    </div>
    )
  }
}
