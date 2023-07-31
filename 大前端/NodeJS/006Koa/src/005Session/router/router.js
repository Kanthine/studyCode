const Router = require('koa-router') 
const router = new Router()
const userRouter = require('./userRouter')
const listRouter = require('./listRouter')
const homeRouter = require('./homeRouter')
const loginRouter = require('./loginRouter')

router.use('/login', loginRouter.routes(), loginRouter.allowedMethods())
router.use('/user', userRouter.routes(), userRouter.allowedMethods())
router.use('/list', listRouter.routes(), listRouter.allowedMethods())
router.use('/home', homeRouter.routes(), homeRouter.allowedMethods())
router.redirect('/', '/home') // 重定向
module.exports= router;