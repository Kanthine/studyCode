import React, {Component, memo} from 'react'

export default class App extends Component {

  state = {
    ch1: '组件一',
    ch2: '组件二',
  };

  render(){    

    return (
      <div>
        <h1>Memo 功能</h1>
        
        <button onClick={()=>{
          this.setState({ch1: "组件 1"})
        }}>刷新组件 1 </button>

        <Child_1 name={this.state.ch1}/>

        <button onClick={()=>{
          this.setState({ch2: "组件 2"})
        }}>刷新组件 2 </button>

        <Child_2 name={this.state.ch2}/>
    </div>
    )
  } 
}

function Child_1(props) {
  let {name} = props;
  console.log('刷新 Child_1 组件')
  return (<div><h2>{name}</h2></div>)
}

const Child_2 = memo((props)=>{
  let {name} = props;
  console.log('刷新 Child_2 组件')
  return (<div><h2>{name}</h2></div>)
})



/** Memo
 * 背景：子组件仅在它的 props 发生改变时进行重新渲染，
 *      通常来讲，在组件树中 React 组件，只要有变化就会走一遍渲染流程
 * 解决：React.memo() 可以仅仅针对某些组件进行渲染
 * 
 *  PureComponent 用于 class 组件
 *  memo 用于函数组件
 */