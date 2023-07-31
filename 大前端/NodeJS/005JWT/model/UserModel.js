const mongoose = require('mongoose')

/*
MongoDB 只有集合和文档，并不像MySQL中有表结构，在MongoDB数据库中的每一条文档可以是完全不一样的数据结构，那么就需要一个规范 —— Schema，
Schema 只是规范存放文档数据类型并没有操作数据库的能力；Schema支持的数据类型有String 、Number、Date、Buffer、Boolean、Array、ObjectId 和 Mixed;
*/ 
const docSchema = new mongoose.Schema({
  username: String,
  password: String,
  avatar: String,
  age: Number
})

/*
Schema 仅定义了文档的框架，让每一个 Schema 都与 MongoDB 数据库中的集合对应，
Schema 和 Model 是定义和生成collection(集合) 和 document(文档) 过程的工具；Model是什么呢？

Schema编译而成的构造器是Model，包含抽象属性和行为，每一个实例化后的Model实际上是一个文档，可以实现对数据库的操作；
通过 model() 方法创建Model时，格式如下：
*/

const UserModel = mongoose.model('user', docSchema)
module.exports = UserModel;