[MySQL]() 关系型数据库!

# 1、介绍

付费的商用数据库：
* Oracle
* SQL Server 微软自家产品，Windows 定制专款；
* DB2、IBM
* Sybase

这些数据库都是不开源、付费的，好处是出了问题可以找商家解决，不过在 web 的世界里，经常需要部署成千上万的数据服务器，当然不能把大把的钱花费在这上面！因此无论Geogle、Facebook，还是国内的 BAT，都选择免费的开源数据库；

* MySQL 使用频率很高的数据库，出了问题可以很容易的找到解决方案；
* PostgreSQL 知名度没有 MySQL 高；
* Sqlite 嵌入式数据库，适合桌面和移动应用；

# 2、与非关系型数据库对比

关系型和非关系型数据库的主要差异是数据存储的方式，你的数据及其特性是选择数据存储和提取方式的首要影响因素：
* 关系型数据天然就是表格式的，因此存储在数据表的行和列中。数据表可以彼此关联协作存储，也很容易提取数据。
* 与其相反，非关系型数据不适合存储在数据表的行和列中，而是大块组合在一起。非关系型数据通常存储在数据集中，就像文档、键值对或者图结构。


关系型数据库最典型的数据结构是表，由二维表及其之间的联系所组成的一个数据组织：
* 优点1、易于维护:都是使用表结构，格式一致; 
* 2、使用方便: SQL语言通用，可用于复杂查询; 
* 3、复杂操作: 支持SQL，可用于一个表以及多个表之间非常复杂的查询。 

* 缺点:1、读写性能比较差，尤其是海量数据的高效率读写; 
* 2、固定的表结构，灵活度稍欠; 
* 3、高并发读写需求，传统关系型数据库来说，硬盘I/O是一个很大的瓶颈。

非关系型数据库严格上不是一种数据库，应该是一种数据结构化存储方法的集合，可以是文档或者键值对等。
* 优点:1、格式灵活:存储数据的格式可以是key,value形式、文档形式、图片形式等等，文档形式、图片形式 等等，使用灵活，应用场景广泛，而关系型数据库则只支持基础型。 
* 2、速度快:nosql可以使用硬盘或者随机存储器作为载体，而关系型数据库只能使用硬盘; 
* 3、高扩展性;
* 4、成本低:nosql数据库部署简单，基本都是开源软件。
* 缺点:1、不提供sql支持;
* 2、无事务处理; 3、数据结构相对复杂，复杂查询方面稍欠。


下载安装 mysql 数据库

[mysql](https://dev.mysql.com/downloads/file/?id=516828)

```
MacBook-Pro $ open ~/.bash_profile
export PATH=/usr/local/mysql/bin:$PATH
MacBook-Pro $ source ~/.bash_profile
```

# 2、mysql 命令


## 2.1、进入数据库操作界面

```
/// 进入 mysql 数据库
MacBook-Pro $ mysql -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 13
Server version: 8.0.32 MySQL Community Server - GPL

Copyright (c) 2000, 2023, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> 
```

## 2.2、数据库操作

```
/// 创建数据库
mysql> create database test;
Query OK, 1 row affected (0.00 sec)

/// 查询本地数据库
mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| test               |
+--------------------+
5 rows in set (0.01 sec)

/// 删除数据库
mysql> drop database test;
Query OK, 0 rows affected (0.00 sec)

/// 进入某个数据库
mysql> use sql_test;
Database changed

/// 退出数据库
mysql> exit
Bye
```

## 2.3、操作表

```
mysql> use sql_test;
Database changed

/// 删除表
mysql> drop table IN_PERIOD;
Query OK, 0 rows affected (0.01 sec)

/// 创建表并初始一个字段
mysql> create table UserTable(userID int auto_increment primary key);
Query OK, 0 rows affected (0.01 sec)

/// 查看表
mysql> show tables;
+--------------------+
| Tables_in_sql_test |
+--------------------+
| UserTable          |
+--------------------+
1 row in set (0.00 sec)

/// 查看字段属性
mysql> desc UserTable;
+--------+------+------+-----+---------+----------------+
| Field  | Type | Null | Key | Default | Extra          |
+--------+------+------+-----+---------+----------------+
| userID | int  | NO   | PRI | NULL    | auto_increment |
+--------+------+------+-----+---------+----------------+
1 row in set (0.00 sec)

/// 添加字段
mysql> alter table UserTable add userName varchar(20) after userID;
Query OK, 0 rows affected (0.01 sec)
Records: 0  Duplicates: 0  Warnings: 0


mysql> desc UserTable;
+----------+-------------+------+-----+---------+----------------+
| Field    | Type        | Null | Key | Default | Extra          |
+----------+-------------+------+-----+---------+----------------+
| userID   | int         | NO   | PRI | NULL    | auto_increment |
| userName | varchar(20) | YES  |     | NULL    |                |
| password | varchar(20) | YES  |     | NULL    |                |
| age      | int         | YES  |     | NULL    |                |
+----------+-------------+------+-----+---------+----------------+
4 rows in set (0.01 sec)

/// 插入数据
mysql> insert into UserTable(userName) values("张三");
Query OK, 1 row affected (0.00 sec)
mysql> insert into UserTable(userName, password, age)  values("张三", "123", 10);
Query OK, 1 row affected (0.00 sec)
mysql> insert into UserTable(userName, password, age)  values("李四", "456", 20);
Query OK, 1 row affected (0.00 sec)


/// 查看表内所有数据
mysql> select * from UserTable;
+--------+----------+----------+------+
| userID | userName | password | age  |
+--------+----------+----------+------+
|      1 | 张三     | 123      |   10 |
|      2 | 李四     | 456      |   20 |
+--------+----------+----------+------+
2 rows in set (0.00 sec)
```