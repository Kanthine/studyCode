/// 性能优化
import React, {PureComponent} from 'react'

class Box extends PureComponent {

  render() {
    let {current, index} = this.props;
    console.log('refresh:', index)
    return (<div style={{width:'100px', height:'100px', 
                 border:current==index?'2px solid red' : '2px solid black', 
                 margin:'10px',float:'left'}}>
    </div>)
  }
}

export default class PureEqual extends PureComponent {

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

/** 性能优化
 * 
 * 1、shouldComponentUpdate() 控制组件或子组件是否需要更新，尤其在子组件非常多的时候，需要进行优化
 * 
 * 2、PureComponent 会自动分析 nextProps 与 prevProps、 nextState 与 prevState 是否相等，
 *                  决定 shouldComponentUpdate() 返回 false 或 true
 *                  从而决定是否调用 render()
 *  注意：如果 state 或者 prev 永远在变，那么 PureComponent 并不会太快，因为 shallowEqual 也需要花费时间
 * 
 */