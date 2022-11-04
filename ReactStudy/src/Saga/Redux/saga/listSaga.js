import { take, fork, put, call} from "redux-saga/effects";

function *listSaga() {
  while(true) {
    // take 监听、组件发来的 action
    yield take('get-list')
    // fork 同步执行异步处理函数
    yield fork(getList)
  }
}

function *getList() {
  /// 异步处理

  // call 函数发出异步请求
  let res = yield call(getListAction);

  yield put({
    type:'change-list',
    payload: res, 
  });
}

function getListAction() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve(['1', '2', '3', '4', '5']);
    }, 2000);
  });
}

export default listSaga; 