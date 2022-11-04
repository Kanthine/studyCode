import React, {Component} from 'react'
import Tabbar from './Child/Tabbar'
import Home from './Child/Home'
import Menu from './Child/Menu'
import Forum from './Child/Forum'
import './CSS/app2.css' /// 导入模块，webpack 的支持
import IndexRouter from './indexRouter'
import store from './Redux/store'

export default class RouterApp extends Component {

  constructor(){
    super()
    this.tabList=[
      {id:1, text:'影院', jump:'/home'},
      {id:2, text:'目录', jump:'/menu'},
      {id:3, text:'电影', jump:'/forum'},
    ];
    this.state = {
      tabIndex:0,
      tabHide: store.getState().tabbarReducer.tabbarHidden,
      isLogin: store.getState().loginReducer.isLogin,
    }
  }

  componentDidMount() {
    store.subscribe((value)=>{
      console.log('App subscribe', store.getState())
      this.setState({
        tabHide: store.getState().tabbarReducer.tabbarHidden,
        isLogin:store.getState().loginReducer.isLogin});
    });
  }

  render(){
    let {tabIndex, isLogin, tabHide} = this.state;

    return (
      <div>
        <IndexRouter isLogin={isLogin}>
          {!tabHide && (<Tabbar list={this.tabList} currentIndex={tabIndex} click={(index)=>{
                this.setState({tabIndex:index});
              }}/>)
          }
        </IndexRouter>

        {/* <Navbar title={this.tabList[tabIndex].text} click={(index)=>{
          console.log(index)
          this.setState({tabIndex:index})
        }}></Navbar>
        {this.switchTabbar()}
      */}

    </div>
    )
  }

  switchTabbar =()=>{
    let {tabIndex} = this.state;
    switch(tabIndex) {
      case 0: return <Home/>
      case 1: return <Menu/>
      case 2: return <Forum/>
    }
  }
}


/**
 * $ npm install react-router-dom@5
 * 
 * 编程式导航、声明式导航
 */
