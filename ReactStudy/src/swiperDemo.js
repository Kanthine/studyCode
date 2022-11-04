import React, {Component} from 'react'
import Swiper from 'swiper'
import 'swiper/css'

export default class SwiperDemo extends Component {

componentDidMount(){
    new Swiper('.swiper')
}

  render(){    
    return (
      <div>

        <div className='swiper'>
            <div className='swiper-wrapper'>
            <div>123456</div>
          <div>aghsehjd</div>
            </div>
        </div>

        {/* <h1>插槽测试</h1>
        <Child>
          <div>123456</div>
          <div>aghsehjd</div>
        </Child> */}
    </div>
    )
  } 
}

class Child extends Component {

  render(){
    return (
      <div>
        {this.props.children[1]}
        {this.props.children[0]}
    </div>)
  } 
}
