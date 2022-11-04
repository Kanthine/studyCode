/** 登陆页面
*/

import React, {Component} from 'react'
import store from '../Mobx/store';

class Login extends Component {
  render(){
    return (<div> 
      <h1>登陆页面</h1>
      <button onClick={ ()=>{
        this.props.login();
        store.isLogin = true;
        // this.props.history.goBack();
        }}>登陆</button>
    </div>)
  }
}

export default Login;