let loginBtn = document.querySelector('#login')
let loginPost = document.querySelector('#loginPost')
let username = document.querySelector('#username')
let password = document.querySelector('#password')

console.log('login', loginBtn)
console.log('username', username)
console.log('password', password)
loginBtn.onclick = ()=>{
    console.log('login get')
    let url = `/user/login?username=${username.value}&passowrd=${password.value}`
    fetch(url)
    .then(res=>res.json())
    .then(res=>{
        location.href="/home"
    })
}

loginPost.onclick = ()=>{
    console.log('login post')
    let url = `/user/login`
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
            console.log('response:', response);
            return response.json()
        } else {
            return Promise.reject(response.json())
        }
    }).then(function(data) {
        console.log('data:', data);
        location.href="/home"
    }).catch(function(err) {
        alert(err)
        console.log('error:', err);
    });
}
