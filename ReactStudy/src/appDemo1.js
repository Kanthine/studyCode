import React, {Component} from 'react'

 /* 下载 axios
  * MacBook App $ npm i axios 
  */
import axios, { Axios } from 'axios'
import './css/appDemo1.css' /// 导入模块，webpack 的支持

/** 选项卡 Demo */


/// RN 官方推拿使用行内样式：每个组件都是一个独立的整体
class Home extends Component {
  constructor(){
    super()
    this.state = {
      list:[],
      searchText:'',
    }
  }

  componentDidMount() {
    /// 请求数据
    // axios.get('https://www.baidu.com').then(result=>{
    //   console.log(result)
    // }).catch(error=>{
    //   console.log(error)
    // }) 

    axios({
      url:'https://m.maizuo.com/gateway?cityId=110100&ticketFlag=1&k=9134111',
      method:'get',
      headers:{
        'X-Client-Info':'{"a":"3000","ch":"1002","v":"5.2.0","e":"1653184925217368294850561"}',
        'X-Host':'mall.film-ticket.cinema.list'
      }
    }).then(result=>{
      console.log('网络数据')
      console.log(result.data.data.cinemas)
      this.setState({
        list:result.data.data.cinemas,
      })
    }).catch(error=>{
      console.log('网络错误')
      console.log(error)
    })
  }

  render(){
    return (
      <div>
        <input value={this.state.searchText} onInput={this.handleInput}/>
        {this.getListData().map(item=>
            <dl key={item.cinemaId}>
                <dt>{item.name}</dt>
                <dd>{item.address}</dd>
            </dl>      
        )}
      </div>
    )
  }

  getListData(){
    let search = this.state.searchText.toUpperCase();
    return this.state.list.filter(item=>
      item.name.toUpperCase().includes(search) ||
      item.address.toUpperCase().includes(search)
    )
  }

  handleInput =(event)=> {
    this.setState({searchText:event.target.value})
  }
}

class Menu extends Component {
  render(){
    return (<div>
      Menu
    </div>)
  }
}

class Forum extends Component {
  render(){
    return (<div>
      论坛
    </div>)
  }
}

export default class APP_1 extends Component {

  constructor(){
    super()
    this.tabList=[
      {id:1, text:'首页'},
      {id:2, text:'目录'},
      {id:3, text:'论坛'},
    ];
    this.state = {
      tabIndex:0
    }
  }

  render(){
    let {tabIndex} = this.state;

    return (
      <div>

        {/* {tabIndex==0 && <Home/>}
        {tabIndex==1 && <Menu/>}
        {tabIndex==2 && <Forum/>} */}
        {this.switchTabbar()}

        <ul>{
          this.tabList.map((item, index)=>
            <li key={item.id} style={{color: tabIndex==index?'red':'black' }} onClick={()=>this.tabBarClick(index)}>{item.text}</li>)
          }</ul> 
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

  tabBarClick=(index)=>{
    console.log(index)
    this.setState({tabIndex:index})
  }

}
