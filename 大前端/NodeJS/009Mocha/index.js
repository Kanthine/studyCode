
function sum(...arr) {
  var sum = 0
  for (let i of arr) {
    sum += i
  }
  return sum
}

module.exports = sum;

/**
 * npm install mocha
 * 
*/