import React, {Component, forwardRef} from 'react'

export default class SlotDemo extends Component {

  myInput2 = React.createRef()

  render(){    
    return (
      <div>
        <h1>引用透传</h1>
        <Child callBack={(ref)=>{
          console.log('callback ', ref)
        }}/>
        <button style={{marginTop:'30'}} onClick={()=>{
          
        }}>获取 Child 的 input 引用</button>

        <Child2 ref={this.myInput2}/>
        <button style={{marginTop:'30'}} onClick={()=>{
            console.log('Child2 ', this.myInput2)
        }}>获取 Child2 的 input 引用</button>
      </div>
    )
  } 
}

class Child extends Component {

  myInput = React.createRef()

  componentDidMount(){
    this.props.callBack(this.myInput);
  }

  render(){
    return (
      <div style={{background:'red'}}>
        <input ref={this.myInput} defaultValue='占位值' />
    </div>)
  } 
}

const Child2 = forwardRef((props, ref)=>{
  return (
    <div style={{background:'blue'}}>
      <input ref={ref} defaultValue='占位值' />
  </div>)
})

/** forwardRef 引用传递
 * 是一种通过组件向子组件自动传递引用的技术，
 * 
 */