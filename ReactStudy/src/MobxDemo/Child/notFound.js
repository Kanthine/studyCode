import React, {Component} from 'react'
import '../CSS/app2.css'

function NotFound(props) {
  console.log('props:', props);
  return (<div>
    <h1> 404 NotFound </h1>
    <h2> 页面不存在 </h2>
  </div>)
}

function handler(pram, obj){
  return (CusComponent)=>{
    return (props)=>{
      return (<div style={{color:'red'}}>
        <CusComponent {...pram()} {...props} {...obj}/>
      </div>) 
    }
  }
}

export default handler(()=>{
  return {
    textColor: 'red',
    backColor: 'gray',
  }
},{
  cusFun1(){},
  cusFun2(){},
})(NotFound);

/** 高阶组件的封装
 * 代码复用、代码模块化
 * 增删改 props
 * 渲染劫持
*/