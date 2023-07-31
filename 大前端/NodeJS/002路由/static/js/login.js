let loginBtn = document.querySelector('#login')
let loginPost = document.querySelector('#loginPost')
let username = document.querySelector('#username')
let password = document.querySelector('#password')

console.log('login', loginBtn)
console.log('username', username)
console.log('password', password)
loginBtn.onclick = ()=>{
    let url = `/api/login?username=${username.value}&passowrd=${password.value}`
    fetch(url)
    .then(res=>res.json())
    .then(res=>{
        console.log('res' ,res)
    })
}
loginPost.onclick = ()=>{
    let url = `/api/loginpost`
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            username:username.value,
            password:password.value
        })
    }).then(response => {
        if (response.status === 200) {
            console.log('response', response);
            return response.text()
        } else {
            console.log('fail', response)
            return Promise.reject(response.json())
        }
    }).then(function(data) {
        console.log('data:', data);
    }).catch(function(err) {
        console.log('error:', err);
    });
}
