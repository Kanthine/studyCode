import React, {Component} from 'react'
import { 
  LaptopOutlined, 
  NotificationOutlined, 
  UserOutlined, 
  DownOutlined, 
  SmileOutlined 
} from '@ant-design/icons';
import { 
  Button, 
  Modal,
  Col, 
  Row, 
  Breadcrumb, 
  Layout, 
  Menu, 
  Dropdown, 
  Space,
  message, 
  Carousel,
  Table,
  Steps ,
  Tree
} from 'antd';
import type { ColumnsType } from 'antd/lib/table'
import type { DataNode, DirectoryTreeProps } from 'antd/lib/tree';
import 'antd/dist/antd.css'
import './css/LayoutPC.css' 

const { Header, Content, Sider, Footer } = Layout;

const items1 = ['1', '2', '3'].map((key) => ({
  key,
  label: `nav ${key}`,
}));

const items2 = [UserOutlined, LaptopOutlined, NotificationOutlined].map((icon, index) => {
  const key = String(index + 1);
  return {
    key: `sub${key}`,
    icon: React.createElement(icon),
    label: `subnav ${key}`,
    children: new Array(4).fill(null).map((_, j) => {
      const subKey = index * 4 + j + 1;
      return {
        key: subKey,
        label: `option${subKey}`,
      };
    }),
  };
});

const kDropdownMenu = (
  <Menu
    items={[
      {
        key: '1',
        label: (<a target="_blank" rel="noopener noreferrer" href="https://www.antgroup.com">home1</a>),
      },
      {
        key: '2',
        label: (<a target="_blank" rel="noopener noreferrer" href="https://www.aliyun.com">home2 (disabled)</a>),
        icon: <SmileOutlined />,
        disabled: true,
      },
      {
        key: '3',
        label: (<a target="_blank" rel="noopener noreferrer" href="https://www.luohanacademy.com">home3 (disabled)</a>),
        disabled: true,
      },
      {
        key: '4',
        danger: true,
        label: 'home4',
      },
    ]}
  />
);


const { Step } = Steps;
const steps = [
  {
    title: '基本信息',
    content: '请填写姓名、性别、年龄',
  },
  {
    title: '兴趣爱好',
    content: '请填写你的兴趣爱好',
  },
  {
    title: '专业特长',
    content: '请填写你的擅长领域',
  },
];

const contentStyle: React.CSSProperties = {
  height: '160px',
  color: '#fff',
  lineHeight: '160px',
  textAlign: 'center',
  background: '#364d79',
};

interface DataType {
  key: React.Key;
  name: string;
  age: number;
  address: string;
  description: string;
}

const columns: ColumnsType<DataType> = [
  { title: 'Name', dataIndex: 'name', key: 'name' },
  Table.EXPAND_COLUMN,
  { title: 'Age', dataIndex: 'age', key: 'age' },
  Table.SELECTION_COLUMN,
  { title: 'Address', dataIndex: 'address', key: 'address' },
];

const data: DataType[] = [
  {
    key: 1,
    name: 'John Brown',
    age: 32,
    address: 'New York No. 1 Lake Park',
    description: 'My name is John Brown, I am 32 years old, living in New York No. 1 Lake Park.',
  },
  {
    key: 2,
    name: 'Jim Green',
    age: 42,
    address: 'London No. 1 Lake Park',
    description: 'My name is Jim Green, I am 42 years old, living in London No. 1 Lake Park.',
  },
  {
    key: 3,
    name: 'Not Expandable',
    age: 29,
    address: 'Jiangsu No. 1 Lake Park',
    description: 'This not expandable',
  }, 
  {
    key: 4,
    name: 'Joe Black',
    age: 32,
    address: 'Sidney No. 1 Lake Park',
    description: 'My name is Joe Black, I am 32 years old, living in Sidney No. 1 Lake Park.',
  },
];

const { DirectoryTree } = Tree;
const treeData: DataNode[] = [
  {
    title: 'parent 0',
    key: '0-0',
    children: [
      { 
        title: 'leaf 0-0', 
        key: '0-0-0', 
        children: [
          { title: 'leaf 1-0-0', key: '0-1-0-0', isLeaf: true },
          { title: 'leaf 1-1-1', key: '0-1-1-1', isLeaf: true },
        ],
        isLeaf: false  // 叶子节点
      },
      { title: 'leaf 0-1', key: '0-0-1', isLeaf: true },
    ],
  },
  {
    title: 'parent 1',
    key: '0-1',
    children: [
      { title: 'leaf 1-0', key: '1-1-0', isLeaf: true },
      { title: 'leaf 1-1', key: '1-1-1', isLeaf: true },
    ],
  },
  {
    title: 'parent 2',
    key: '0-2',
    children: [
      { title: 'leaf 2-0', key: '2-1-0', isLeaf: true },
      { title: 'leaf 2-1', key: '2-1-1', isLeaf: true },
    ],
  },
];


export default class LayoutUI extends Component {

  constructor(){
    super()
    this.state = {
      breadcrumbIndex:0,
      currentStep: 0,
    }
  }

  next = () => {
    this.setState({currentStep: this.state.currentStep + 1});
  };

  prev = () => {
    this.setState({currentStep: this.state.currentStep - 1});
  };

  onSelect: DirectoryTreeProps['onSelect'] = (keys, info) => {
    console.log('Trigger Select', keys, info);
  };

  onExpand: DirectoryTreeProps['onExpand'] = (keys, info) => {
    console.log('Trigger Expand', keys, info);
  };

  handleCancel = () => {
    this.setState({breadcrumbIndex:0});
  };

  renderContext=()=> {
    let {breadcrumbIndex, currentStep} = this.state;
    if (breadcrumbIndex == 1) {
      return (<Carousel autoplay effect="fade">
          <div><h1 style={contentStyle}>1</h1></div>
          <div><h1 style={contentStyle}>2</h1></div>
          <div><h1 style={contentStyle}>3</h1></div>
          <div><h1 style={contentStyle}>4</h1></div>
        </Carousel>
      );
    } else if (breadcrumbIndex == 2) {
      return <>
        <Steps current={currentStep}>{steps.map(item => (<Step key={item.title} title={item.title} />))}</Steps>
        <div className="steps-content">{steps[currentStep].content}</div>
        <div className="steps-action">
          {currentStep <   steps.length - 1 && (<Button type="primary" onClick={() => this.next()}>下一步</Button>)}
          {currentStep === steps.length - 1 && (<Button type="primary" onClick={() => message.success('Processing complete!')}>完成</Button>)}
          {currentStep >   0 && (<Button style={{ margin: '0 8px' }} onClick={() => this.prev()}>上一步</Button>)}
        </div>
      </>
    } else if (breadcrumbIndex == 3) {
      return <DirectoryTree multiple defaultExpandAll onSelect={this.onSelect} onExpand={this.onExpand} treeData={treeData}/>
    } else if (breadcrumbIndex == 4) {
      return <Modal title="Basic Modal" visible={true} onOk={this.handleCancel} onCancel={this.handleCancel}>
            <p>Some contents...</p>
            <p>Some contents...</p>
            <p>Some contents...</p>
          </Modal>
    } else {
      return (<Table columns={columns} rowSelection={{}} dataSource={data}
        expandable={{
          expandedRowRender: record => <p style={{ margin: 0 }}>{record.description}</p>,
        }}/>)
    }
  }


  render(){
    let {breadcrumbIndex} = this.state;

    return (
    <Layout>
      <Header className="header">
        <div className="logo" />
        <Menu theme="dark" mode="horizontal" defaultSelectedKeys={['2']} items={items1} />
      </Header>
      <Layout>
        <Sider width={200} height={100} className="site-layout-background">
          <Menu mode="inline" defaultSelectedKeys={['1']} defaultOpenKeys={['sub1']} style={{height: '100%', borderRight: 0,}} items={items2}/>
        </Sider>
        <Layout style={{padding: '0 24px 24px', backgroundColor:'red'}}>
          <Breadcrumb style={{margin: '16px 0',}}>
            <Breadcrumb.Item><Dropdown overlay={kDropdownMenu}><a onClick={e => e.preventDefault()}><Space>Tableb表<DownOutlined /></Space></a></Dropdown></Breadcrumb.Item>
            <Breadcrumb.Item  onClick={()=>{
              this.setState({breadcrumbIndex:1});
            }}>轮播图</Breadcrumb.Item>
            <Breadcrumb.Item onClick={()=>{
              this.setState({breadcrumbIndex:2});
              console.log('完善信息 Click')
            }}>完善信息</Breadcrumb.Item>
            <Breadcrumb.Item onClick={()=>{
              this.setState({breadcrumbIndex:3});
            }}>树形控件</Breadcrumb.Item>
          </Breadcrumb>
          <Breadcrumb.Item onClick={()=>{
              this.setState({breadcrumbIndex:4});
            }}>Modal组件</Breadcrumb.Item>
          <Content className="site-layout-background" style={{padding: 16,margin: 0,minHeight: 460}}>{this.renderContext()}</Content>
          <Footer style={{ textAlign: 'center' }}>Ant Design ©2022 Created by Ant UED</Footer>
        </Layout>
      </Layout>
    </Layout>)
  }
}