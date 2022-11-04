import React, {Component} from 'react'
import Tabbar from './Child/Tabbar'
import Navbar from './Child/Navbar'
import Home from './Child/Home'
import Menu from './Child/Menu'
import Forum from './Child/Forum'
import './CSS/app2.css' /// 导入模块，webpack 的支持


export default class APP_2 extends Component {

  constructor(){
    super()
    this.tabList=[
      {id:1, text:'影院'},
      {id:2, text:'目录'},
      {id:3, text:'电影'},
    ];
    this.state = {
      tabIndex:0
    }
  }

  render(){
    let {tabIndex} = this.state;

    return (
      <div>
        <Navbar title={this.tabList[tabIndex].text} click={(index)=>{
          console.log(index)
          this.setState({tabIndex:index})
        }}></Navbar>
        {this.switchTabbar()}
        <Tabbar list={this.tabList} currentIndex={tabIndex} click={(index)=>{
          this.setState({tabIndex:index})
        }}/>
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
