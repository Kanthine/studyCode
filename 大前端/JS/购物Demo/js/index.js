const user = JSON.parse(window.localStorage.getItem('user'))
const isLogin = !!user

if( isLogin) {
    $('.header span').text(user.username)
    $('.off').removeClass('active')
    $('.on').addClass('active')
} else {
    $('.off').addClass('active')
    $('.on').removeClass('active')
}

$('button.goSelf').on('click', function(){
    window.location.href = './personal.html'
})

$('button.logout').on('click', function(){
    window.localStorage.setItem('user', null)
    window.alert('退出登录, 跳转登录页面')
    window.location.href = './login.html'
    console.log('退出登录')
})