import React, {Component} from 'react'


/// 函数式组件
const Navbar = (props)=> {
  let {title, click} = props;
  return (<div style={{background:'yellow', textAlign:'center', overflow:'hidden'}}>
    <button style={{float:'left'}} onClick={()=>{click(0)}}>back</button>
    <span>{title}</span>
    <button style={{float:'right'}} onClick={()=>{click(2)}}>My</button>
  </div>)
}

export default Navbar