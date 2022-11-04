import { takeEvery, put, call} from "redux-saga/effects";

function *listSaga() {
  yield takeEvery('get-list', getList);
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

export {listSaga, getList}; 