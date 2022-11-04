import { observable, autorun, action, configure} from 'mobx';

configure({
  enforceActions: 'always',
})

class StoreC {
  @observable isLogin = false;
  @observable isTabshow = true;

  @action tabbarShow=()=>{
    this.isTabshow = true;
    console.log(' tabbarShow ', this.isTabshow);
  }

  @action tabbarHide=()=>{
    this.isTabshow = false;
    console.log(' tabbarHide ', this.isTabshow);
  }
}

const store = new StoreC();
export default store;

// const store = observable({
//   isLogin: false,
//   isTabshow: true,
// });


/**
 * 引入 Mobx介绍
 * $ npm install mobx
 * $ npm install mobx-react
 * 
 * Mobx介绍
 *    Mobx是一个功能强大，上手非常容易的状态管理工具。
 *    Mobx背后的哲学很简单: 任何源自应用状态的东西都应该自动获得；
 *    Mobx利用getter和setter来收集组件的数据依赖关系，从而在数据发生变化的时候精确知道哪些组件需要重绘，在界面的规模变大的时候，往往会有很多细粒度更新。
 * 
 * 
 * 支持装饰器
 * 1、配置中开启 experimentalDecorators
 * 2、下载  npm install @babel/core @babel/plugin-proposal-decorators @babel/preset-env
 * 3、创建 .babelrc
 */
