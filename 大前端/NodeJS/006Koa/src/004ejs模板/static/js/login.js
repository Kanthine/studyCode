let loginBtn = document.querySelector('#login')
let loginPost = document.querySelector('#loginPost')
let username = document.querySelector('#username')
let password = document.querySelector('#password')

console.log('login', loginBtn)
console.log('username', username)
console.log('password', password)
loginBtn.onclick = ()=>{
    let url = `/user?username=${username.value}&passowrd=${password.value}`
    fetch(url)
    .then(res=>res.json())
    .then(res=>{
        console.log('res' ,res)
    })
}

loginPost.onclick = ()=>{
    let url = `/user`
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
            return response.json()
        } else {
            return Promise.reject(response.json())
        }
    }).then(function(data) {
        console.log('data:', data);
        if (data.code == 0) {
            location.href = './home.html'
        } else {
            
        }
    }).catch(function(err) {
        console.log('error:', err);
    });
}
