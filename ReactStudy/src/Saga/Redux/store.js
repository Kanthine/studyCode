import React, {Component} from 'react'
import { createStore, applyMiddleware} from 'redux'
import reducer from './reducer'
import createSageMiddleware from 'redux-saga'
import watchSaga from './saga/saga'
import everySaga from './saga_every/saga'

const kSaga = createSageMiddleware();
const store = createStore(reducer, applyMiddleware(kSaga));

// kSaga.run(watchSaga); // saga 任务
kSaga.run(everySaga); // saga 任务

export default store;