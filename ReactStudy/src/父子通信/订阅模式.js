import React, {Component} from 'react'


export default class DispathMessage extends Component {
  render(){
    return (
      <div>
        <h1>订阅模式</h1>
    </div>
    )
  }
}

/// 调度中心
let DispathCenter = {
  list:[],

  /// 订阅
  subscribe(callback) {
    this.list.push(callback)
  },

  /// 发布
  publish(value){
    this.list.forEach(callback => {
      callback && callback(value)
    });
  }
}

DispathCenter.subscribe((value)=>{
  console.log('111111', value)
})

DispathCenter.subscribe((value)=>{
  console.log('2222222',value)
})

setTimeout(()=>{
  DispathCenter.publish('广播形参')
}, 1)

/// redux 基于订阅发布