import React, {useState} from 'react'

export default function HOOK1() {

  const [text, setText] = useState('')
  const [list, setList] = useState([0, 1, 2, 3, 4, 5, 6])

  return (
    <div>
      <h1>插槽测试</h1>
      <input value={text} onChange={(event)=>{
        setText(event.target.value)
      }}/>
      <button onClick={()=>{
        setList([...list, list.length])
      }}>Add List</button>

      {list.map((item)=><li key={item}>{item}</li>)}
  </div>
  )
}

/** hook 之 useState 的使用：保存组件状态
 *      通过 set 方法保存状态，然后整个函数组件都会重新执行
 */