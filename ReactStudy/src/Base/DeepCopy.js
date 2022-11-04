import React, {Component} from 'react'
import {Map} from 'immutable'

let obj1 = {
  name : 'Tom', 
  arr: [1, 2, 3, 4],
};
console.log('oj1', obj1);

/// 浅拷贝
let obj2 = obj1
console.log('obj2', obj2);
obj2.name = 'John'
obj2.arr.push(5) 

console.log('oj1', obj1, 'obj2', obj2);

/// 比浅拷贝多一层，但容器类不能深拷贝
let obj3 = {...obj2}
obj3.name = 'Hello'
obj3.arr.push(6) 

console.log('oj1', obj1, 'obj2', obj2, 'obj3',obj3);

/// 实现彻底的深拷贝
/// 缺陷：obj 中不能含有 undefined ,否则会丢失对应的字段
let obj4 = JSON.parse(JSON.stringify(obj3))
obj4.name = 'Json'
obj4.arr.push(7) 
console.log('oj1', obj1, '\nobj2', obj2, 'obj3',obj3, 'obj4', obj4);


let obj_json1 = {
  name : undefined, 
  arr: [1, 2, 3, 4],
};

let obj_json2 = JSON.parse(JSON.stringify(obj_json1))
// obj_json2.name = 'Json'
obj_json2.arr.push(5) 
console.log('obj_json1', obj_json1, '\nobj_json2', obj_json2);


/** 深度拷贝：将对象的每一个子节点拷贝一份；使得两个对象的值修改，互不影响；
 *           缺点：所有节点复制一份，占用内存，导致性能损耗
 *  
 * Immutable 库：
 *    实现原理： Persistent Data Structure(持久化数据结构)，也就是使用旧数据创建新数据时，要保证旧数据同时可用且不变。
 *    同时为了避免 deepCopy 把所有节点都复制一遍带来的性能损耗，Immutable 使用 了 Structural Sharing(结构共享)，
 *    即如果对象树中一个节点发生变化，只修改这个节点和受它影响的父节点， 其它节点则进行共享。
 *    原理动图： https://upload-images.jianshu.io/upload_images/2165169-cebb05bca02f1772
 * 
 * 下载 Immutable ： npm install immutable
*/


let obj_i_1 = {
  name: 'Tom',
  address : undefined, 
  arr: [1, 2, 3, 4],
};

let obj_i_2 = Map(obj_i_1);
let obj_i_3 = obj_i_2.set('address', '上海') /// set 修改字段值
obj_i_3.get('name') /// get 获取字段值
console.log('obj_i_1', obj_i_1)
console.log('obj_i_2', obj_i_2.toJS())
console.log('obj_i_3', obj_i_3.toJS())

export default class DeepCopy extends Component {

  state = {
    info: Map({name: 'Tom', age : 20,}),
  }
  
  render(){    
    let {info} = this.state;
    let age = this.state.info.get('age');
    return (
      <div>
        <h1>深拷贝</h1>
        <button onClick={()=>{
          this.setState({info: info.set('age', age + 1).set('name', 'Tom' + age)});
        }}>增加年龄</button>
        <h2>{info.get('name')}</h2>
        <h2>{info.get('age')}</h2>
    </div>
    )
  } 
}