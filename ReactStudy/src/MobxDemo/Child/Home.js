import React, {Component} from 'react'

 /* 下载 axios
  * MacBook App $ npm i axios 
  */
import axios, { Axios } from 'axios'
import '../CSS/app2.css' /// 导入模块，webpack 的支持

export default class Home extends Component {

  constructor(){
    super()

    this.state = {
      list:[],
      searchText:'',
    }
  }

  componentDidMount() {
    /// 请求数据
    axios.get(`/list.json`).then(result=>{
      console.log('网络数据',result)
      this.setState({
        list:result.data.data.cinemas,
      })
    }).catch(error=>{
      console.log('网络错误', error)
    })

    // axios({
    //   url:'https://m.maizuo.com/gateway?cityId=110100&ticketFlag=1&k=9134111',
    //   method:'get',
    //   headers:{
    //     'X-Client-Info':'{"a":"3000","ch":"1002","v":"5.2.0","e":"1653184925217368294850561"}',
    //     'X-Host':'mall.film-ticket.cinema.list'
    //   }
    // }).then(result=>{
    //   console.log('网络数据')
    //   console.log(result.data.data.cinemas)
    //   this.setState({
    //     list:result.data.data.cinemas,
    //   })
    // }).catch(error=>{
    //   console.log('网络错误')
    //   console.log(error)
    // })
  }

  render(){
    return (
      <div style={{bottom:'50'}}>
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
