const mongoose = require('mongoose')

mongoose.set('strictQuery', true)

// 插入集合或数据，数据库 appUsersTest 自动创建
mongoose.connect('mongodb://127.0.0.1:27017/appUsersHeader', (error)=>{
  if(error) {
    console.log('mongoose --- 链接 --- ', error)
  } else {
    console.log('mongoose --- 链接 --- 成功')
  }
}) 
