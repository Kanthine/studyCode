import { createStore } from 'redux'

function show() {
  return {
    type: 'tabbar_show'
  }
}

function hide() {
  return {
    type: 'tabbar_hide'
  }
}

export {show, hide};
