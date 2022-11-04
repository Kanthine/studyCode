import {takeEvery} from "redux-saga/effects";
import {pwdSaga, getAccount} from "./pwdSaga";
import {listSaga, getList} from "./listSaga";
import {chainSaga, getChainList} from "./链式Saga"; 

function *everySaga() {
  yield takeEvery('get-Account', getAccount);
  yield takeEvery('get-list', getList);
  yield takeEvery('get-chain', getChainList);
}

export default everySaga;