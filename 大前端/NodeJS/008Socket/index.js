var express = require('express');
var app = express();

app.use(express.static('./static'))
app.get('/', (req, res)=>{
  res.send({
    code:1,
    data:{
      username:'张三',
      age:18
    }
  })
})

app.listen(3200);







/// 模拟后端服务器
const WebSocket = require('ws')
const WebSocketServer =  WebSocket.WebSocketServer
const wss = new WebSocketServer({ port: 3201 });
wss.on('connection', function connection(ws) {
  ws.on('error', console.error);
  ws.on('message', function message(data) {
    console.log('前端发来消息: %s', data);
    // 转发给其它打开的前端
    wss.clients.forEach(function each(client) {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(data, { binary: true });
      }
    });
  });
  ws.send('some data');
});

/**
 * npm install mocha
 * 
*/