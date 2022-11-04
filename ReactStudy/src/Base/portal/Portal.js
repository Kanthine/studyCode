import React, {Component} from 'react'
import { createPortal } from 'react-dom'; 

export default class App extends Component {

  state = {
    isShow: false,
  }

  render(){    
    let {isShow} = this.state;
    return (
      <div className='box'>
        <h1 align="center">Portal</h1>
                
        <button onClick={()=>{
          this.setState({isShow: !isShow})
        }}>弹出 Modal</button>

        {/* {isShow && <Dialog/>} */}
        {isShow && <ProtalDialog onClose={()=>{
          this.setState({isShow: !isShow})
        }}/>}
    </div>
    )
  } 
}

class Dialog extends Component {

  render(){
    return (
      <div style={{width:'100%', height:'100%', position:'fixed', left:0, top:0, backgroundColor:'rgba(0.0, 0.0, 0.0, 0.3)'}}>
        <h1>Dialog</h1>
        <button onClick={this.props.onClose}>关闭 Modal</button>
    </div>)
  } 
}

/// 通过 createPortal() 函数将组建传送到根节点之下
class ProtalDialog extends Component {
  render(){
    return createPortal(
      <div style={{width:'100%', height:'100%', position:'fixed', left:0, top:0, backgroundColor:'rgba(0.0, 0.0, 0.0, 0.3)'}}>
        <div style={{width:'50%', height:'50%', position:'fixed', left:'25%', top:'25%', backgroundColor:'white', textAlign:'center'}}>
          <h1>ProtalDialog</h1>
          <button onClick={this.props.onClose}>关闭 Modal</button>
        </div>
    </div>, document.body)
  } 
}

/** Portal
 */