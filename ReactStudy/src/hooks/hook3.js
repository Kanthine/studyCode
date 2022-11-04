import React, {useContext, useState, useEffect} from 'react'
import axios, { Axios } from 'axios'

const globalContext = React.createContext();

export default function HOOK3() {

  const [list, setList] = useState([])
  const [item, setItem] = useState({})

  useEffect(()=>{
    axios.get(`/movies.json`).then(result=>{
      console.log('网络数据',result)
      setList(result.data.data.films);
    }).catch(error=>{
      console.log('网络错误', error)
    })
  }, []);
  
  return (<globalContext.Provider value={{
      call:'打电话',
      sms:'短信通信',
      item: item,
      changeItem:(item)=>{
        setItem(item);
      }
    }}>
    <div>
      {list.map(item=>
          <dl key={item.filmId}><FilmItem item={item}/></dl>      
      )}
      <FilmDetaile/>
    </div>
  </globalContext.Provider>)
}

function FilmItem(props) {
  let {item} = props;
  const value = useContext(globalContext);
  return <div className='files' onClick={()=>{ value.changeItem(item)}}>
    <dt>{item.name}</dt>
    <img src={item.poster} alt={item.name}/>
    <dd>{item.category}</dd>
  </div>  
}

function FilmDetaile() {
  const value = useContext(globalContext);
  return (<div className='files'>
    <h2>详情数据</h2>
    <dt>{value.item.name}</dt>
    <img src={value.item.poster} alt={value.item.name}/>
    <dd>{value.item.category}</dd>
  </div>) 
}

/** hook 之 useState 的使用：保存组件状态
 *      通过 set 方法保存状态，然后整个函数组件都会重新执行
 */