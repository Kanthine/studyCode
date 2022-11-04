import React, {Component} from 'react'
import axios, { Axios } from 'axios'


const globalContext = React.createContext();

export default class ContenxtDemo extends Component {

  constructor() {
    super()
    this.state = {
      list:[],
      item:{},
    }
  }
  
  componentDidMount() {
    axios.get(`/movies.json`).then(result=>{
      console.log('网络数据',result)
      this.setState({
        list:result.data.data.films,
      })
    }).catch(error=>{
      console.log('网络错误', error)
    })
  }

  render(){
    return (<globalContext.Provider value={{
      call:'打电话',
      sms:'短信通信',
      item:this.state.item,
      changeItem:((item)=>{
        this.setState({item:item})
      })
    }}>
      <div>
          {this.state.list.map(item=>
              <dl key={item.filmId}>
                  <FilmItem item={item}/>
              </dl>      
          )}
          <FilmDetaile/>
      </div>
    </globalContext.Provider>)
  }
}


class FilmItem extends Component {
  render() {
    let {item} = this.props;
    return (<globalContext.Consumer>{ (value)=> {
      console.log(value)

      return <div className='files' onClick={()=>{ value.changeItem(item)}}>
        <dt>{item.name}</dt>
        <img src={item.poster} alt={item.name}/>
        <dd>{item.category}</dd>
      </div>
    }} 
    </globalContext.Consumer>)
  }
}


class FilmDetaile extends Component {

  constructor(){
    super()
    this.state = {
      item:{},
    }
  }

  render() {
    let {item} = this.state;
    return <globalContext.Consumer>
    { (value)=>
      (<div className='files'>
          <h2>详情数据</h2>
          <dt>{value.item.name}</dt>
          <img src={value.item.poster} alt={item.name}/>
          <dd>{value.item.category}</dd>
      </div>)
    }</globalContext.Consumer>
  }
}

/** context 状态树传参：可以跨越多个组件进行通信
 * 
*/