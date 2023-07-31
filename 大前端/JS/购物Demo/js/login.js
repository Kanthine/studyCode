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

$('form').on('submit', function(e){
    // 阻止默认行为
    e.preventDefault()

    // 采集用户信息
    const data = $('form').serializeObject()
    // 发送请求
    if (!data.username || !data.password) {
        $('form > span').text('请填写账号或者密码')
        $('form > span').css('display', 'block')
    } else {
        $.get(`http://localhost:3100/users?username=${data.username}&password=${data.password}`, (data, status)=> {
            if(status == 'success') {
                let users = [...data]
                if(users.length) {
                    $('form > span').css('display', 'none')
                    window.localStorage.setItem('user', JSON.stringify(users[0]))
                    window.alert('登录成功, 跳转首页')
                    window.location.href = './index.html'
                } else {
                    $('form > span').text('账户名或者密码错误')
                    $('form > span').css('display', 'block')
                }
            } else {
                $('form > span').css('display', 'block')
            }
        })
    }
})