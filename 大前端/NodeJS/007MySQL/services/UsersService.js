const config = {
  host: '127.0.0.1',
  port: '3306',
  user: 'root',
  password: '12345678',
  database: 'sql_test',
  connectionLimit: 1,
}
const mysql2 = require('mysql2')
const promisePool = mysql2.createPool(config).promise();

const UserService = {
  addUser: async (username, password, age) => {
    const users = await promisePool.query(`insert into UserTable (userName, userID, age) values (?, ?, ?)`, [username, password, age])
    return users[0]
  },

  deleteUser: async (userID) => {
    const users = await promisePool.query(`delete from UserTable where userID=?`, [userID])
    return users[0]
  },

  getUsers: async () => {
    const users = await promisePool.query('select * from UserTable')
    return users[0]
  },

  updateUser: async (userID, username, password, age) => {
    const users = await promisePool.query(`update UserTable set userName=? where userID=?`, [username, userID])
    return users[0]  
  },

  login: async (username, password) => {
    const users = await promisePool.query(`select * from UserTable where userName="${username}" and password="${password}"`)
    return users[0]  
  },
}

module.exports = UserService;
