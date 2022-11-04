import { takeEvery, put, call} from "redux-saga/effects";

function *chainSaga() {
  yield takeEvery('get-chain', getChainList);
}

function *getChainList() {
  /// 异步处理

  // call 函数发出异步请求
  let res1_1 = yield call(getListAction1_1);
  let res1_2 = yield call(getListAction1_2, res1_1);

  yield put({
    type:'change-Chain',
    payload: res1_2, 
  });
}

function getListAction1_1() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve(['1', '2', '3', '4', '5']);
    }, 2000);
  });
}

function getListAction1_2(data) {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve([...data, 'a', 'b', 'c', 'd', 'e']);
    }, 2000);
  });
}

export {chainSaga, getChainList}; 