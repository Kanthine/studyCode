/** 登陆页面
*/

import React, {Component} from 'react'
import { login } from '../Redux/Action/LoginAction';
import store from '../Redux/store';

export default class Login extends Component {
  render(){
    return (<div> 
      <h1>登陆页面</h1>
      <button onClick={ ()=>{
        store.dispatch(login());
        // this.props.history.goBack();
        }}>登陆</button>
    </div>)
  }
}


