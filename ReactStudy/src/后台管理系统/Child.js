import React, {Component} from 'react'
import styles from './css/Child.modules.css'


// console.log('style == :', styles.item)
export default class Child extends Component {

  render(){    
    return (
      <div>
        <h1>Child</h1>
        <ul>
          <li className='item1'>----------1----------</li>
          <li className={styles.item}>----------2----------</li>
          <li>----------3----------</li>
        </ul>
    </div>
    )
  } 
}
