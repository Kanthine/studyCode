import React, {Component} from 'react'

export default class Tabbar extends Component {
  render(){
    let {list, currentIndex, click} = this.props;
    return (<div>
      <ul>{
          list.map((item, index)=>
            <li key={item.id} style={{color: currentIndex==index?'red':'black' }} onClick={()=>click(index)}>{item.text}</li>)
      }</ul> 
    </div>)
  }
}
