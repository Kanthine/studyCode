import React, {Component} from 'react'

export default class ClickDemo extends Component {

  count = 100

  render(){
    return (
      <div>
        <input/>
        /*
           写法一：最大的弊端是不能处理逻辑复杂的事件，否则代码行数太多，
           不推荐此种写法，适用于少量逻辑代码
           */
        <button onClick={ ()=>{
            ///匿名函数
            console.log("匿名函数",this.count)

        }}>btn1</button>

        // bind 仅仅是改变 this 指向，而不会执行该函数
        // 不推荐这种写法
        <button onClick={this.click2.bind(this)}>btn2</button>
        <button onClick={this.click3}>btn3</button>

        // 强烈推荐该写法：非常适合传参数
        <button onClick={ ()=>this.click4() }>btn4</button>

        // 错误写法: 事件函数后带()
        // render 会执行内部带有() 的语句
        // this.click2() 没有返回值，即返回是未定义的 undefined
        // 该句代码实际是 <button onClick={undefined}>btn2</button>
        // <button onClick={this.click2()}>btn2</button>

    </div>
    )
  }

  // click2 被事件系统调用，所以 this 指向事件系统，而不是 ClickDemo 实例
  click2(){
    console.log("写法2",this.count)
  }
  // 箭头函数，不会关注谁调用，this 指向永远外部作用域，也就是 ClickDemo 实例
  click3 = ()=>{
    console.log("写法3",this.count)
  }
  click4 = ()=>{
    console.log("写法4",this.count)
  }
}

/**
React 必不会真正的绑定事件到每一个具体的 《》 元素上，而是采用事件代理的模式；


*/
