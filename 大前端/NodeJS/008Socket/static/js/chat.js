const ws = new WebSocket('ws://localhost:3201')

ws.onopen = ()=>{
    console.log('连接成功')
}

ws.onmessage = (msg)=>{
    console.log('msg ========== ', msg, msg.data)

    ws.send('to server')
}

ws.onerror = (err)=>{
    console.log('err ========== ', err)
}