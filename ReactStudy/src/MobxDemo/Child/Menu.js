import React, {Component} from 'react'

export default class Menu extends Component {
  render(){
    let detaile_id = 123;
    return (<div>
        <button onClick={ ()=>{
          
          /// 方案一：动态路由传惨
          //this.props.history.push(`/detaile/${detaile_id}`)

          /// 方案二：query 传参
          // this.props.history.push({
          //   pathname:'/detaile',
          //   query:{
          //     detaileID: detaile_id,
          //   }
          // })

          /// 方案三：state 传参
          this.props.history.push({
            pathname:'/detaile',
            state:{
              detaileID: detaile_id,
            }
          })

        }}>详情界面</button>
    </div>)
  }
}