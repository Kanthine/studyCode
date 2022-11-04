import React, {Component} from 'react'

function step1() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve('step1');
    }, 1000);
  });
}

function step2() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve('step2');
    }, 1000);
  });
}

function step3() {
  return new Promise((resolve, reject)=>{
    setTimeout(() => {
      resolve('step3');
    }, 1000);
  });
}

function *gen() {
  var f1 = yield step1();
  console.log('f1 ', f1)
  var f2 = yield step2(f1);
  console.log('f2 ', f2)
  var f3 = yield step3(f2);
  console.log('f3 ', f3)
} 

function run(fn) {
  var g = fn();

  function next(data) {
    var res = g.next(data);
    if (res.done) {
      return res.value
    } 
    res.value.then(r1=>{
      next(r1);
    })
  }
  next()
}

run(gen);
