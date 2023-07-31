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

let user = JSON.parse(window.localStorage.getItem('user'))
if (!user) {
    window.location.href('./login.html')
} else {
    $('form [name=username]').val(user.username)
    $('form [name=password]').val(user.password)
    $('form [name=repassword]').val(user.password)
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
        let info = {username: data.username, password: data.password}
        var param = JSON.stringify(info)
        var xhr3 = new XMLHttpRequest()
        xhr3.onload = function(event) {
            // window.localStorage.setItem('user', JSON.stringify(users[0]))
            window.localStorage.setItem('user', xhr3.responseText)
            console.log(JSON.parse(xhr3.responseText))
            $('form > span').css('display', 'none')
            window.alert('修改成功, 跳转首页')
            window.location.href = './index.html'
        }
        xhr3.onerror = function(event) {
            $('form > span').text('修改失败')
            $('form > span').css('display', 'block')
        }
        xhr3.open('PATCH', `http://localhost:3100/users/${user.id}`, true)
        xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
        xhr3.send(param)
    }
})