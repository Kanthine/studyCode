import React, {Component} from 'react'
import Tabbar from './Child/Tabbar'
import Navbar from './Child/Navbar'
import Home from './Child/Home'
import Menu from './Child/Menu'
import Forum from './Child/Forum'
import NotFound from './Child/notFound'
import Detaile from './Child/Detaile'
import Login from './Child/Login'
import { HashRouter, BrowserRouter, Route, Switch} from 'react-router-dom'
import { Redirect } from 'react-router-dom'

export default function IndexRouter(props) {
  let isLogin = props.isLogin;
  return <BrowserRouter>
    {props.children}
    {props.children}
    {props.children}
    <Switch>
      <Route path='/home' component={Home}/>
      <Route path='/menu' component={Menu}/>
      <Route path='/login' component={Login}/>

      {/* 路由拦截 */}
      <Route path='/forum' render={(props)=>{
        console.log('Route: ', props);
        return isLogin ? <Forum/> : <Redirect to='/login'/>
        // return localStorage.getItem('isLogin') == '1' ? <Forum {...props}/> : <Redirect to='/login'/>
      }}/>

      
      {/* 动态路由 */}
      {/* 方案一：动态路由传参 */}
      {/* <Route path='/detaile/:detaileID' component={Detaile}/> */}

      {/* 方案二：路由：数据结构传参 */}
      <Route path='/detaile' component={Detaile}/>

      {/* 路由重定向  精确匹配 exact */}
      <Redirect from='/' to='/menu' exact/>
      
      <Route component={NotFound}/>
    </Switch>
  </BrowserRouter>
}


/**
 * $ npm install react-router-dom@5
 */
