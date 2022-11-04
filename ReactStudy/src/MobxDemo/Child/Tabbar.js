import React, {Component} from 'react'
import { NavLink } from 'react-router-dom';
// import tabStyle from '../CSS/tabbarStyle.css'
import styled from 'styled-components';

const StyleFooter = styled.footer`
  background:gray;
  left:10px;
  right:10px;
  bottom:0;
  height:50px;
  text-align:center;
  ul{
    display:flex;
    li{
      flex:1;
      &:hover{
        background:blue;
      }
    }
  }
`
 
/// 声明式导航、实现方案2：NavLink 实现
export default class Tabbar extends Component {
  render(){
    let {list,} = this.props;
    return (<StyleFooter> 
      <ul>{
          list.map((item, index)=>
            <li key={item.id}>
              <NavLink to={item.jump} 
                    //activeClassName={tabStyle.tabActive} // 选中样式
                    >{item.text}</NavLink>
            </li>
            )
      }</ul> 
    </StyleFooter>)
  }
}

// const styles = StyleSheet.create({
//   danmuDataLine: {
//       width: 0.5,
//       backgroundColor: '#0000001A',
//       backgroundColorInDark: '#FFFFFF0D',
//   },
// });



// 声明式导航、实现方案1: a href 弊端：通过路由跳转， tabbar item 无法高亮
// export default class Tabbar extends Component {
//   render(){
//     let {list, currentIndex, click} = this.props;
//     return (<div> 
//       <ul>{
//           list.map((item, index)=>
//             <li key={item.id} style={{backgroundColor: currentIndex==index?'red':'gray' }} onClick={()=>click(index)}>
//               <a href={item.jump}>{item.text}</a>
//             </li>
//             )
//       }</ul> 
//     </div>)
//   }
// }
{/* <Route path='/home' component={Home}/>
<Route path='/menu' component={Menu}/>
<Route path='/forum' component={Forum}/> */}



/**
 * 编程式导航
 * 声明式导航
 * 
 * npm install styled-components
 * npm install react-test-renderer
*/