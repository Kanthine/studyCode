var moduleA = require('./a')
var moduleB = require('./b')
var moduleC = require('./c')
console.log(moduleA.common(), moduleB.initB(), moduleC())
