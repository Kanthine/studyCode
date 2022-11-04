import React, {Component} from 'react'
import { Button, Space, Swiper, Toast, Tabs} from 'antd-mobile'
import './css/LayoutPhone.css' 

const colors = ['#ace0ff', '#bcffbd', '#e4fabd', '#ffcfac']

const items = colors.map((color, index) => (
  <Swiper.Item key={index}>
    <div className="Swiper-content" style={{ background: color }} onClick={() => {
        Toast.show(`你点击了卡片 ${index + 1}`)
      }}
    >
      {index + 1}
    </div>
  </Swiper.Item>
))

const UI_Items = [
  <Tabs.Tab title='Button' key='Button'>Button</Tabs.Tab>,
  <Tabs.Tab title='Swiper' key='Swiper'>Swiper</Tabs.Tab>
];

export default class PhoneUI extends Component {

  constructor(){
    super()
    this.state = {
      btnstatus: false,
    }
  }

  render(){
    let text = "测试状态"
    // 获取输入框的引用
    return (
      <div>
        <h1>Phone_UI 组件库·Button</h1>
        <Space wrap>
          <Button color='primary' fill='solid'>Solid</Button>
          <Button color='primary' fill='outline'>Outline</Button>
          <Button color='primary' fill='none'>None</Button>
        </Space><br/><br/>

        <Space wrap>
          <Button shape='default' color='primary'>Default Button</Button>
          <Button block shape='rounded' color='primary'>Rounded Button</Button>
          <Button block shape='rectangular' color='primary'>Rectangular Button</Button>
        </Space><br/><br/>

        <Space wrap>
          <Button loading color='primary' loadingText='正在加载'>Loading</Button>
          <Button loading>Loading</Button>
          {/* <Button loading='auto' onClick={async () => { await sleep(1000)}}>Auto Loading</Button> */}
        </Space><br/><br/>

        <h1>Phone_UI 组件库·轮播</h1>
        <Swiper loop>{items}</Swiper>

        <Tabs defaultActiveKey='1'>
          {UI_Items}
        </Tabs>

        <Tabs defaultActiveKey='1'>
          <Tabs.Tab title='Espresso' key='1'>
            1
          </Tabs.Tab>
          <Tabs.Tab title='Coffee Latte' key='2'>
            2
          </Tabs.Tab>
          <Tabs.Tab title='Cappuccino' key='3'>
            3
          </Tabs.Tab>
          <Tabs.Tab title='Americano' key='4'>
            4
          </Tabs.Tab>
          <Tabs.Tab title='Flat White' key='5'>
            5
          </Tabs.Tab>
          <Tabs.Tab title='Caramel Macchiato' key='6'>
            6
          </Tabs.Tab>
          <Tabs.Tab title='Cafe Mocha' key='7'>
            7
          </Tabs.Tab>
        </Tabs>

    </div>
    )
  }
}

/**
 * yarn add antd-mobile
 */