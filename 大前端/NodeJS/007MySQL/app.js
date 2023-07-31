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




// const config = {
//   host: '127.0.0.1',
//   port: '3306',
//   user: 'root',
//   password: '12345678',
//   database: 'sql_test',
//   connectionLimit: 1,
// }
// const mysql2 = require('mysql2')
// const promisePool = mysql2.createPool(config).promise();
// async function testSQL() {
//   /// 查所有数据
//   const users = await promisePool.query('select * from UserTable')
//   console.log('users ===========', users[0])

//   /// 查指定数据
//   let name = '张三'
//   let userID = '1'
//   const users1 = await promisePool.query(`select * from UserTable where userName="${name}"`)
//   console.log('users1 ===========', users1[0])

//   const users2 = await promisePool.query(`select * from UserTable where userName="${name}" and userID="${userID}"`)
//   console.log('users2 ===========', users2[0])

//   // 插入
//   const users3 = await promisePool.query(`insert into UserTable (userName, userID) values (?, ?)`, [name, userID])
//   console.log('users3 ===========', users3[0])

//     // 更新
//   const users4 = await promisePool.query(`update UserTable set userName=? where userID=?`, [name, userID])
//   console.log('users4 ===========', users4[0])

//   // 删除
//   const users5 = await promisePool.query(`delete from UserTable where userID=?`, [userID])
//   console.log('users5 ===========', users5[0])
// }
// testSQL()




module.exports = app;


/**
 * npm install mysql2
 * 
*/