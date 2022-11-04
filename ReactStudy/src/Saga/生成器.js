import React, {Component} from 'react'

function *test() {
  console.log('--- 1 ---')
  console.log('--- 2 ---')
  yield '中断 1';
  console.log('--- 3 ---')
}

var kTest = test()

var res1 = kTest.next()
var res2 = kTest.next()

console.log('res1 ', res1)
console.log('res2 ', res2)
