/* 数据存储在内存中
*/
const tabbarReducer = (prevState={tabbarHidden: false}, action)=> {
  let newState = {...prevState}
  switch(action.type) {
    case 'tabbar_hide':
      newState.tabbarHidden = true
      break;
    case 'tabbar_show':
      newState.tabbarHidden = false
      break;
    default :break;
  }
  console.log('tabbarReducer: ', action, newState);
  return newState;
}

export default tabbarReducer;


/**
 * Redux 的数据缓存在内存中，如果想要存储在硬盘，需要额外操作
 * 
 * npm install redux-persist
 * 
*/