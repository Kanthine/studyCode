import { observable, autorun } from 'mobx';

const value = observable.box(10);
const name = observable.box('Tom');

autorun(()=>{
  /// 只扫自家门前雪、哪管他人瓦上霜
  console.log('get value:', value.get());
})

autorun(()=>{
  /// 只有对应值修改，才会调用 autorun() 
  console.log('get name: ', name.get());
})

setTimeout(()=>{
  // value.set(20);
  name.set('章三');
}, 1000)


let obj = observable.map({
  name: 'Tom',
  age : 18,
});

autorun(()=>{
  console.log('get obj: ', obj.get('name'));
})

setTimeout(()=>{
  obj.set('name', '李四');
}, 1000)

let obj2 = observable({
  name: 'Tom',
  age : 18,
});

autorun(()=>{
  console.log('get obj2: ', obj2.name);
})

setTimeout(()=>{
  obj2.name = '王五';
}, 1000)






/**
 * 引入 mobx
 * $ npm install mobx
 * 
 * Mobx介绍
 *    Mobx是一个功能强大，上手非常容易的状态管理工具。
 *    Mobx背后的哲学很简单: 任何源自应用状态的东西都应该自动获得；
 *    Mobx利用getter和setter来收集组件的数据依赖关系，从而在数据发生变化的时候精确知道哪些组件需要重绘，在界面的规模变大的时候，往往会有很多细粒度更新。
 */
