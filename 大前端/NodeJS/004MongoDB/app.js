var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');
var loginRouter = require('./routes/login');

var session = require('express-session')
var MongoStore = require('connect-mongo')

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// 注册 session 中间件
app.use(session({
  name: 'nodeTest',
  secret: 'set key',
  cookie:{
    maxAge: 24 * 60 * 60 * 1000, /// 单位毫秒
    secure: false /// 是否仅支持 https
  },
  resave: true, /// 从最后一次访问开始计算过期时间 （否则从第一次访问计算过期时间）
  saveUninitialized: true, /// 初始化一个无效 session
  store: MongoStore.create({ // npm install connect-mongo
    mongoUrl: 'mongodb://127.0.0.1:27017/nodeTest_session', //新创建了一个数据库
    ttl: 24 * 60 * 60 * 1000 // 过期时间
  }),
}))

/// 设置一个应用级别中间件，用于校验 cookie
app.use((req, res, next) => {
  console.log('session', req.session.user)
  if(req.url.includes('login')) {
    next()
  } else if(req.session.user) { // 校验 session  
    req.session.mydate = Date.now() /// 重置过期时间
    next()  
  } else if(req.url.includes('api')){  
    res.status(401).json({code:1})
  } else {
    res.redirect('/login')  
  }
});

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/login', loginRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use((err, req, res, next) =>{
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;


/* 业务分层
 * routers 路由负责将请求分发给 controller 层
 * controller 层负责处理业务逻辑：View 于 Model 的沟通
 *      从 view 层拿到前端传过来的数据，交给 Model 层处理
 *      从 Model 层请求到的数据，交给 view 层渲染
 * view 层负责展示渲染数据
 * model 对数据库的增删改查
*/


// app.use((req, res, next) => {
//   //排除login相关的路由和接口
//   if (req.url.includes("login")) {
//     next()
//     return
//   }

//   const token = req.headers["authorization"]?.split(" ")[1]
//   if(token){
//     const payload=  JWT.verify(token)
//     if(payload){
//       //重新计算token过期时间
//       const newToken = JWT.generate({
//         _id:payload._id,
//         username:payload.username
//       },"1d")
//       res.header("Authorization",newToken)
//       next()
//     }else{
//       res.status(401).send({errCode:-1,errInfo:"token过期"})
//     }
//   }else{
//     next()
//   }
// })
