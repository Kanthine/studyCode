import { take, fork, put, call} from "redux-saga/effects";

function *pwdSaga() {
  while(true) {
    // take 监听、组件发来的 action
    yield take('get-Account')
    // fork 同步执行异步处理函数
    yield fork(getAccount)
  }
}

function *getAccount() {
  /// 异步处理

  // call 函数发出异步请求
  let res = yield call(getAccountAction);

  yield put({
    type:'change-Account',
    payload: res, 
  });
}

function getAccountAction() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve({'name':'张三', 'password':'123456'});
    }, 2000);
  });
}

export default pwdSaga; 