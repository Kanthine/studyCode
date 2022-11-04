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
