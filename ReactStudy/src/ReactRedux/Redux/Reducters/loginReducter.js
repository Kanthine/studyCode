/* 数据存储在内存中
*/
const loginReducer = (prevState={isLogin : false}, action)=> {
  let newState = {...prevState}
  switch(action.type) {
    case 'login':
      // localStorage.setItem('isLogin','1');
      newState.isLogin = true
      break;
    case 'logout':
      // localStorage.setItem('isLogin', '0');
      newState.isLogin = false
      break;
    default :break;
  }
  console.log('loginReducer: ', action, newState);
  return newState;
}

export default loginReducer;
