import { combineReducers, createStore, compose} from 'redux'
import loginReducer from './Reducters/loginReducter'
import tabbarReducer from './Reducters/TabbarReducter'


/* 几个扩展 Reducer 合并为一个大 Reducer
*/
const reducer = combineReducers({
  loginReducer,
  tabbarReducer
}) 

const composeEnhancers = window.__REDUX_DEVTOOLS_EXTENSION_COMPOSE__ || compose;
const store = createStore(reducer, /* preloadedState, */ composeEnhancers())
// const store = createStore(reducer);

export default store;

/**
 * 引入 redux
 * $ npm install redux
 * 
 * redux 使用的三大原则
 *  state 以单一对象存储在 store 中 ：修改、访问的必须是同一对象；
 *  state 只读（每次返回一个新的对象）：
 *  使用纯函数 reducer 执行 state 更新
 * 
 * 纯函数：
 *  对外界没有副作用
 *  同样的输入、得到同样的输出
 * 
 * Redux 调试插件
 * https://github.com/zalmoxisus/redux-devtools-extension
 */
