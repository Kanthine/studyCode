import { createStore } from 'redux'

function login() {
  return {
    type: 'login'
  }
}

function logout() {
  return {
    type: 'logout'
  }
}

export {login, logout};
