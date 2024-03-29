/** 详情页面
*/

import React, {Component} from 'react'
import store from '../Mobx/store';

class Detaile extends Component {

  componentDidMount() {
    /// 进入页面：隐藏 Tabbar
    store.tabbarHide();
  }
  
  componentWillUnmount() {
    /// 离开页面：显示 Tabbar
    store.tabbarShow();
  }

  render(){
    console.log('Detaile: ', this.props);
    return (<div> 
      <h1>详情页面</h1>

      {/* 方案三：state 传参 */}
      <h2>{this.props.location.state.detaileID}</h2>

      {/* 方案二：query 传参 */}
      {/* <h2>{this.props.location.query.detaileID}</h2> */}

      {/* 方案一：动态路由传惨 */}
      {/* <h2>{this.props.match.params.detaileID}</h2> */}

      <button onClick={ ()=>{
        this.props.history.goBack();
        }}>返回</button>
    </div>)
  }
}

export default Detaile;