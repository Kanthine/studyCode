import React, {Component} from 'react'
import Child from './Child'
import style from './css/App.css'
console.log('app: ', style)
export default class App extends Component {



  render(){    
    return (
      <div>
        <h1>完整 Demo</h1>
        <ul>
          <li>----------1----------</li>
          <li>----------2----------</li>
        </ul>
        <Child/>
        
        {/* <button onClick={()=>{}}>异步执行 账户</button> */}

    </div>
    )
  } 
}
