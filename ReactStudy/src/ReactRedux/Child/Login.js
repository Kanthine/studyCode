/** 登陆页面
*/

import React, {Component} from 'react'
import { connect } from 'react-redux';
import { login } from '../Redux/Action/LoginAction';

class Login extends Component {
  render(){
    return (<div> 
      <h1>登陆页面</h1>
      <button onClick={ ()=>{
        this.props.login();
        // this.props.history.goBack();
        }}>登陆</button>
    </div>)
  }
}

const mapDispatchToProps = {login};
export default connect(null, mapDispatchToProps)(Login);