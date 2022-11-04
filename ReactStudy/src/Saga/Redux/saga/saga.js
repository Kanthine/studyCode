import {all} from "redux-saga/effects";
import pwdSaga from "./pwdSaga";
import listSaga from "./listSaga";
import chainSaga from "./链式Saga";

function *watchSaga() {
  yield all([listSaga(), pwdSaga(), chainSaga()]);
}

export default watchSaga;