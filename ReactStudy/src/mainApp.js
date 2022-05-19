
import React, {Component} from 'react'
import './css/mainApp.css' /// 导入模块，webpack 的支持

/// RN 官方推拿使用行内样式：每个组件都是一个独立的整体
class Navbar extends Component {
  render(){
    let count = 10
    let name = "Class"
    let styles = {
      backgroundColor:"yellow",
      fontSize:"30px"
    }
    return (
      <div>
        <div style={{background:"yellow"}} >{"My name is" + name + count}</div>
        <div style={{background:"gray"}} >{10 > 20 ? "10 大" : "20大"}</div>
        <div style={styles}>style 后跟一个对象</div>
        <div className="active">使用 CSS</div>
      </div>
    )
  }
}

/// 通过函数创建的组件
function Swiper() {
  // 事件绑定
  return <div>Swiper</div>
}

/// ES6 的箭头函数
/// this 指向和外部作用域的 this 指向保持一致，在事件绑定的时候可能引发 this 大战
const Tabbar = ()=> {
  return (<div>Tabbar</div>)
}

/// 每个模块只允许一个默认导出 export default
/// 根组件 APP
/// 放在谁的组件中，谁就是谁的子组件
export default class APP extends Component {
  render(){
    return (
      <div>
        <Navbar></Navbar>
        <Swiper></Swiper>
        <Tabbar></Tabbar>
    </div>
    )
  }
}
