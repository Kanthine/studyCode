// setState 同步与异步的问题

import React, {Component} from 'react'
import TPropTypes from 'prop-types'

class Navbar extends Component {
  /// 使用 static 声明类属性
  static propTypes = {
    title:TPropTypes.string,
    leftshow: TPropTypes.bool
  }
  /// 设置默认值
  static defaultProps = {
    leftshow:true
  } 

  render() {
    console.log(this.props);
    let {title, leftshow} = this.props; /// 解构操作
    return (<div>
        {leftshow && (
          <div>leftBtn</div>
        )}
        <div>{title}</div>
    </div>)
  }
}

/// 类属性: 传入的属性必须符合下述类型，否则会警告
// Navbar.propTypes = {
//   title:TPropTypes.string,
//   leftshow: TPropTypes.bool
// }

/// 类属性：设置默认值
// Navbar.defaultProps = {
//   leftshow:true
// }




/// 通过函数创建的组件
function Swiper(props) {
  // 事件绑定
  console.log(props)
  let {bg} = props;
  return <div style={{background:bg}}>Swiper</div>
}


export default class PrposDemo extends Component {

  render(){

    let navObj = {
      title:'通过对象传递属性',
      leftshow:false
    }

    return (
      <div>
        {/* <Navbar title="ni" leftshow="q"/> */}
        <div>
          <h2>上</h2>
          <Navbar title='首页'/>
        </div>

        <div>
          <h2>中</h2>
          <Navbar title='论坛' leftshow={false}/>
        </div>

        <div>
          <h2>下</h2>
          {/* 通过展开式写法，将属性传递到 Navbar */}
          <Navbar {...navObj}/>
        </div>

        <div>
          <h2>函数式组件</h2>
          <Swiper bg='red'/>
        </div>

      </div>
    )
  }
}

/**
 * prpos 正常是外部传入、组件内部也可以通过一些方式来初始化的设置；
 * 属性不能被组件自己更改，但是可以通过父组件主动渲染的方式来传入新的 prpos；
 * 
 * prpos 是描述性的、特点性的，组件自己不能随意更改；
 * 
 * 示例·Navbar 属于子组件，父组件通过 title、leftshow 向子组件传递信息
 *      可以隐式使用属性
 *      也可以显示声明属性、规范属性类型、设置属性默认值
 */



/** 属性 VS 状态
 * 
 * 相同点：都是纯 js 对象，都会触发 render 更新，都具有确定性（状态/属性相同，结果相同）
 *        父组件将属性更新到子组件，子组件也会更新
 * 不同点：
 *      1、属性能从父组件获取、状态不能；
 *      2、属性可以被父组件修改，状态不能；
 *      3、属性可以在组件内部设置默认值，状态也可以，但设置方式不同；
 *      4、属性不能在组件内部修改，状态可以在组件内部修改
 *      5、属性能设置子组件初始值。状态不可以
 *      6、属性可以修改子组件的值，状态不可以
 * 
 * 状态 state 的主要作用用于组件保存、控制、修改自己的可变状态；
 *      在组件内部初始化，可以被组件自身修改，而外部不能访问、不能修改；
 *      可以认为 state 是一个局部的、只能被组件自身控制的数据源；
 *      state 中状态可以通过 this.setState() 更新，这会触发 render() 重新渲染
 * 
 * 属性 props 的主要作用是让使用该组件的父组件可以传入参数来配置该组件，它是外部传递进来的配置参数；
 *      属性被设计为：在组件内部仅有只读权限、无写入权限；
 *      组件内部无法控制、修改；除非父组件主动传入新的 props，否则组件的 props 永远保持不变；
 *  
 * 没有 state 的组件叫做无状态组件，设置了 state 的组件叫做有状态组件；
 * 因为状态会带来管理的复杂度，React 提倡子组件多使用无状态组件、少使用有状态组件；
 * 这样会降低代码维护的难度，也在一定程度上增强组件的可复用性；
 * 
*/  