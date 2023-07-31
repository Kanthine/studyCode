$.fn.serializeObject = function()
{
   var o = {};
   var a = this.serializeArray();
   $.each(a, function() {
       if (o[this.name]) {
           if (!o[this.name].push) {
               o[this.name] = [o[this.name]];
           }
           o[this.name].push(this.value || '');
       } else {
           o[this.name] = this.value || '';
       }
   });
   return o;
};

function registerUser(info, callback) {
    var param = JSON.stringify(info)
    var xhr3 = new XMLHttpRequest()
    xhr3.onload = function() {
        callback(JSON.parse(xhr3.responseText))
    }
    xhr3.open('POST', 'http://localhost:3100/users', true)
    xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
    xhr3.send(param)
}

function findUser(info, callback) {
    $.get(`http://localhost:3100/users?username=${info.username}`, (data, status)=> {
        let isHave = false;
        if(status == 'success') {
            let users = [...data]
            if(users.length) {
                isHave = true;
            } 
        }
        callback(isHave)
    })
}

$('form').on('submit', function(e){
    // 阻止默认行为
    e.preventDefault()

    // 采集用户信息
    const data = $('form').serializeObject()
    // 发送请求
    if (!data.username || !data.password || !data.repassword) {
        $('form > span').text('请填写账号或密码')
        $('form > span').css('display', 'block')
    } else if(data.password != data.repassword) {
        $('form > span').text('两次输入密码不一致')
        $('form > span').css('display', 'block')
    } else {
        let info = {username:data.username, password:data.password};
        findUser(info, (isHave)=>{
            if(isHave) {
                $('form > span').text('该用户名已注册')
                $('form > span').css('display', 'block')
            } else {
                $.post('http://localhost:3100/users', info, (data, status)=> {
                    if(status == 'success') {
                        $('form > span').css('display', 'none')
                        window.alert('注册成功, 跳转登录页面')
                        window.location.href = './login.html'
                    } else {
                        $('form > span').text('注册失败')
                        $('form > span').css('display', 'block')
                    }
                })
            }
        })
    }
})