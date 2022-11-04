import React, {Component} from 'react'
// import './生成器'
// import './可执行生成器'
import store from './Redux/store'

export default class App extends Component {

  state={
    inputText:'',
    list:['1','2','3','4','5','6']
  }


  render(){    
    return (
      <div>
        <h1>Saga</h1>
        <button onClick={()=>{
          if(store.getState().list.length == 0) {
            store.dispatch({
              type:'get-list',
            });
          } else {
            console.log('异步获取数据 ', store.getState().list);
          }
        }}>异步执行 list</button>

        <button onClick={()=>{
          if(!!store.getState().account) {
            store.dispatch({
              type:'get-Account',
            });
          } else {
            console.log('异步获Account ', store.getState().account);
          }
        }}>异步执行 账户</button>

        <button onClick={()=>{
          if(store.getState().chain.length == 0) {
            store.dispatch({
              type:'get-chain',
            });
          } else {
            console.log('链式调用 ', store.getState().chain);
          }
        }}>链式调用</button>
    </div>
    )
  } 
}

/**
 * npm install redux-saga
 * 
 * 单元测试：每次版本迭代、快速的进行回归测试
 * 
 * 
*/