// 插槽
import React, {Component} from 'react'
import gl from './global'
// import moment from 'moment';
// import AsyncStorage from '@react-native-community/async-storage';

export default class SlotDemo extends Component {

  render(){    
    console.log('gloabl : ', gl.sex.man)

    let value = localStorage.getItem('AwardTipDateKey')
    if (value != null) {
      console.log(value)
    }else {
      console.log('is null')
      localStorage.setItem('AwardTipDateKey',Date.now())
    }

    return (
      <div>
        <h1>插槽测试</h1>
                
        <Child>
          <div>123456</div>
          <div>aghsehjd</div>
        </Child>
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

/** 插槽 slot
 * 1、为了复用
 * 2、一定程度上，减少父子通信
 */