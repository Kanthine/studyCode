import React, {useContext, useReducer} from 'react'

/// 处理函数
const reducer=()=> {

}

/// 外部对象
const initstate = {
  count : 0,

}

export default function HOOK4() {

  const [state, dispatch] = useReducer(reducer, initstate)

  return <div>
    <h1>{state.count}</h1>
  </div>
}

/** hook 之 useState 的使用：保存组件状态
 *      通过 set 方法保存状态，然后整个函数组件都会重新执行
 */