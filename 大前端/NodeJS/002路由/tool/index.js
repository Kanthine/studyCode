const {server} = require('./server')
const router = require('./route')
const apiRouter = require('./api')

server.use(router)
server.use(apiRouter)
server.start()


// let str = "Hello Word"
// let str2 = str.replace('Hello', "hl")
// console.log(str2)