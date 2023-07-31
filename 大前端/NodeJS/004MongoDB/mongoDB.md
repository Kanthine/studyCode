[MongoDB](https://www.mongodb.com/home) 非关系型数据库!
* 轻量级、自由度高；
* 采用 Bson 格式存储数据，数据友好可见；

# 1、安装 MongoDB

## 1.1、安装包下载

* 1、官网下载 [安装包](https://www.mongodb.com/try/download/community) ！
* 2、将文件存储在 `/usr/local/` 目录下！
* 3、配置环境变量

```
MacBook-Pro $ vim ~/.bash_profile

export PATH=/usr/local/mongodb/bin:$PATH

MacBook-Pro $ source ~/.bash_profile
```

* 4、验证

```
MacBook-Pro $ mongod -version
db version v6.0.3
Build Info: {
    "version": "6.0.3",
    "gitVersion": "f803681c3ae19817d31958965850193de067c516",
    "modules": [],
    "allocator": "system",
    "environment": {
        "distarch": "x86_64",
        "target_arch": "x86_64"
    }
}
```

* 5、启动数据库

```
MacBook-Pro $ mongod --dbpath /usr/local/mongodb/data/db
```

## 1.2、brew 下载

### 1.2.1、下载

```
/// 用tap命令关联第三方的仓库
MacBook-Pro $ brew tap mongodb/brew 
/// 安装最新稳定版
MacBook-Pro $ brew install mongodb-community
/// 卸载
MacBook-Pro $ brew uninstall mongodb-community
```

### 1.2.2、启动数据库

```
// 在后台启动数据库
MacBook-Pro $ brew services start mongodb/brew/mongodb-community
// 停止数据库
MacBook-Pro $ brew services stop mongodb-community 
// 重启数据库
MacBook-Pro $ brew services restart mongodb 
// 不需要在后台启动
MacBook-Pro $ mongod --config /usr/local/etc/mongod.conf 
```

### 1.2.3、进入/退出数据库

```
// 进入mango服务器（服务器必须先启动才能进入），此时可以查看当前mongo服务版本等信息
MacBook-Pro $ mongosh 
Current Mongosh Log ID:	639bc670689782fbbc619832
Connecting to:		mongodb://127.0.0.1:27017/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.6.1
Using MongoDB:		6.0.1
Using Mongosh:		1.6.1

/// 正常退出数据库
test> exit
```

### 1.2.4、文件路径

```
/usr/local/etc/mongod.conf      // 配置文件
/usr/local/var/log/mongodb      // 日志目录路
/usr/local/var/mongodb          // 数据目录路径
```

# 2、常用命令

```
MacBook-Pro $ mongosh
Current Mongosh Log ID:	639bcc5c592729115bd04250
Connecting to:		mongodb://127.0.0.1:27017/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+1.6.1
Using MongoDB:		6.0.1
Using Mongosh:		1.6.1

/// help 命令
test> help
    use dbName    切换数据库，如果没有则创建
    show databases / show dbs :     打印所有可见数据库（不包含新创建的空数据库）
    'show collections'/'show tables': Print a list of all collections for current database.
    'show profile': Prints system.profile information.
    'show users': Print a list of all users for current database.
    'show roles': Print a list of all roles for current database.
    'show log <type>': log for current connection, if type is not set uses 'global'
    'show logs': Print all logs.
    exit                                       Quit the MongoDB shell with exit/exit()/.exit
    quit                                       Quit the MongoDB shell with quit/quit()
    Mongo                                      Create a new connection and return the Mongo object. Usage: new Mongo(URI, options [optional])
    connect                                    Create a new connection and return the Database object. Usage: connect(URI, username [optional], password [optional])
    it                                         result of the last line evaluated; use to further iterate
    version                                    Shell version
    load                                       Loads and runs a JavaScript file into the current shell environment
    enableTelemetry                            Enables collection of anonymous usage data to improve the mongosh CLI
    disableTelemetry                           Disables collection of anonymous usage data to improve the mongosh CLI
    passwordPrompt                             Prompts the user for a password
    sleep                                      Sleep for the specified number of milliseconds
    print                                      Prints the contents of an object to the output
    printjson                                  Alias for print()
    convertShardKeyToHashed                    Returns the hashed value for the input using the same hashing function as a hashed index.
    cls                                        Clears the screen like console.clear()
    isInteractive                              Returns whether the shell will enter or has entered interactive mode
```

## 2.1、数据库相关

```
test> use l_test /// 创建并切换数据库
switched to db l_test

l_test> db /// 查询当前所在数据库

l_test> db.dropDatabase()   /// 删除数据库
{ ok: 1, dropped: 'l_test' }
```

## 2.2、集合相关

```
l_test> db.createCollection('userlist') /// 创建一个集合
{ ok: 1 }

/// 查看集合
l_test> show tables
goodslist
newslist
userlist

/// 查看集合
l_test> db.getCollectionNames()
[ 'goodslist', 'newslist', 'userlist' ]

/// 删除一个集合
l_test> db.goodslist.drop()
true
```

## 2.3、表相关

```
/// 插入数据
l_test> db.userlist.insert({"username":"张三", "age": 18, "sex":true})
DeprecationWarning: Collection.insert() is deprecated. Use insertOne, insertMany, or bulkWrite.
{
  acknowledged: true,
  insertedIds: { '0': ObjectId("639bd3be592729115bd04251") }
}

/// 插入一组数据
l_test> db.userlist.insert([{"username":"李四", "age": 28, "sex":false}, 
                            {"username":"王五", "age": 38, "sex":false}])
{
  acknowledged: true,
  insertedIds: {
    '0': ObjectId("639bd44f592729115bd04253"),
    '1': ObjectId("639bd44f592729115bd04254")
  }
}

/// 修改数据
l_test> db.userlist.updateOne( {"username":"张三"}, 
                               {$set: {"age": 8}, // 重置属性     
                               {$inc: {"age": 8}, // 年龄 +8
                                $unset: {"sex":false} // 删除某个属性
                                }
                             )

/// 查看数据
l_test> db.userlist.find()
[
  {
    _id: ObjectId("639bd3be592729115bd04251"),
    username: '张三',
    age: 8
  },
  {
    _id: ObjectId("639bd3fe592729115bd04252"),
    username: '李四',
    age: 28,
    sex: false
  }
]

/// 删除所有 username 为 李四 的数据
l_test> db.userlist.remove({"username":"李四"})

/// 删除所有数据
db.userlist.remove({})

```



db.userlist.updateOne( {"username":"张三"}, {$set: {"age": 8}, $unset: {"sex":false}})