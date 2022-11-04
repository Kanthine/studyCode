import React, {useEffect, useState} from 'react'
import axios, { Axios } from 'axios'

export default function HOOK2() {

  const [list, setList] = useState([])

  useEffect(()=>{
    axios.get(`/movies.json`).then(result=>{
      console.log('网络数据',result)
      setList(result.data.data.films)
    }).catch(error=>{
      console.log('网络错误', error)
    })
  },[])
  
  return (
    <div>
      <h1>插槽测试</h1>

      {/* <button onClick={()=>{
        setList([...list, list.length])
      }}>Add List</button> */}
      {list.map((item)=><li key={item.filmId}>{item.name}</li>)}
  </div>
  )
}

/** hook 之 useState 的使用：保存组件状态
 *      通过 set 方法保存状态，然后整个函数组件都会重新执行
 * 
 *  useEffect 在函数中第一次执行一次，之后‘依赖‘也会执行更新
 */