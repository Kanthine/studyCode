import React, {Component} from 'react'
import { Button, Col, Row } from 'antd';
import 'antd/dist/antd.css'

export default class AppUI extends Component {

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
        <h1>UI PC 组件库</h1>
        <Button type="primary">Primary Button</Button> <br /><br />
        <Button>Default Button</Button> <br /><br />
        <Button type="dashed">Dashed Button</Button> <br /><br />
        <Button type="text">Text Button</Button> <br /><br />
        <Button type="link">Link Button</Button>  <br /><br />

        <Row>
          <Col span={24} style={{backgroundColor:'blue', color:'white'}}>col</Col>
        </Row>
        <Row>
          <Col span={12} style={{backgroundColor:'blue', color:'white'}}>col-12</Col>
          <Col span={12} style={{backgroundColor:'red', color:'white'}}>col-12</Col>
        </Row>
        <Row>
          <Col span={8} style={{backgroundColor:'blue', color:'white'}}>col-8</Col>
          <Col span={8} style={{backgroundColor:'red', color:'white'}}>col-8</Col>
          <Col span={8} style={{backgroundColor:'black', color:'white'}}>col-8</Col>
        </Row>
    </div>
    )
  }
}

/**
 * npm install antd
 */