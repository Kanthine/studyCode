import React, {Component} from 'react'

export default class SheetDemo extends Component {

  state = {
    show:false
  }
  render(){    
    return (
      <div>
        <h1>测试 Sheet</h1>
        <button onClick={()=>{
          this.setState({show: !this.state.show})
        }}>{this.state.show ? "去隐藏" : "去展示"}</button>
    </div>
    )
  } 
}



class Sheetshow extends Component {

  render(){    
    return (
      <div>

    </div>
    )
  } 
}

