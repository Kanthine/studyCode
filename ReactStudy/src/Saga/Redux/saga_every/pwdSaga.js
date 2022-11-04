import { takeEvery, put, call} from "redux-saga/effects";

function *pwdSaga() {
  yield takeEvery('get-Account', getAccount);
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

export {pwdSaga, getAccount}; 