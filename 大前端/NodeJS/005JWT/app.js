var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');
var loginRouter = require('./routes/login');
var uploadRouter = require('./routes/upload');
const JWT = require('./unit/JWT');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

/// 设置一个应用级别中间件，用于校验 token
app.use((req, res, next) => {
  if(req.url.includes('login')) {
    next()
    return
  }
  const token = req.headers["authorization"]?.split(" ")[1]
  if(token) { // 校验 token  
    const paylod = JWT.verify(token)
    if (paylod) {
      var info = {_id: paylod._id, username: paylod.username}
      const newToken = JWT.generate(info, '1d')
      res.header('Authorization', newToken)
      next()
    } else {
      res.status(401).send({errCode:-1, message:'token 过期'})
    }
  } else {
    next()
  }
});

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/login', loginRouter);
app.use('/upload', uploadRouter);

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