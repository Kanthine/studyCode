import React, {Component} from 'react'
import axios, { Axios } from 'axios'
import '../CSS/app2.css'

export default class Forum extends Component {

  constructor() {
    super()
    this.state = {
      list:[],
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
    return (<div>
        {this.state.list.map(item=>
            <dl key={item.filmId}>
                <FilmItem item={item}/>
            </dl>      
        )}
        <FilmDetaile/>
    </div>)
  }
}


class FilmItem extends Component {
  render() {
    let {item} = this.props;
    return (<div className='files' onClick={()=>{
      DispathCenter.publish(item)
    }}>
      <dt>{item.name}</dt>
      <img src={item.poster} alt={item.name}/>
      <dd>{item.category}</dd>
  </div>)
  }
}


class FilmDetaile extends Component {

  constructor(){
    super()
    this.state = {
      item:{},
    }
    DispathCenter.subscribe((value)=>{
      console.log('111111', value)
      this.setState({
        item:value,
      })
    })

  }

  render() {
    let {item} = this.state;
    return (<div className='files'>
      <dt>{item.name}</dt>
      <img src={item.poster} alt={item.name}/>
      <dd>{item.category}</dd>
  </div>)
  }
}



/// 调度中心
let DispathCenter = {
  list:[],

  /// 订阅
  subscribe(callback) {
    this.list.push(callback)
  },

  /// 发布
  publish(value){
    this.list.forEach(callback => {
      callback && callback(value)
    });
  }
}

/// 基于订阅发布