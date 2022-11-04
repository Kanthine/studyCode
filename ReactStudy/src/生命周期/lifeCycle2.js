/// 生命周期·测试 shouldComponentUpdate() 函数

import React, {Component} from 'react'

class Box extends Component {

  shouldComponentUpdate(nextProps){
    let {current, index} = this.props;

    if(current==index || /// 更新上一次相等的
      nextProps.index==nextProps.current) { /// 更新这一次相等的
      return true
    }
    return false
  }
  render() {
    let {current, index} = this.props;
    console.log('refresh:', index)
    return (<div style={{width:'100px', height:'100px', 
                 border:current==index?'2px solid red' : '2px solid black', 
                 margin:'10px',float:'left'}}>
    </div>)
  }
}

export default class LifeDemo2 extends Component {

  state = {
    list:[0, 1, 2, 3, 4, 5, 6, 7, 8, 9],
    currentIdx:0
  }

  render(){
    return (<div>
        <h1>生命周期</h1>
        <input type="number" value={this.state.currentIdx}
          onChange={(event)=>{ this.setState({currentIdx:Number(event.target.value)}) }}/> 
        <div style={{overflow:'hidden'}}>
          {
            this.state.list.map((item, index)=>
              <Box key={item} current={this.state.currentIdx} index={index}/>)
          }
        </div>
      </div>)
  } 

}
