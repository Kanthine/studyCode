const goodID = window.localStorage.getItem('goodID')

if(!goodID) {
    window.location.href('./list.html')
}

function getGoodDetail(goodID) {
    console.log('goodID', goodID)
    $.get(`http://localhost:3100/goods?id=${goodID}`, (data, status)=> {
        if(status == 'success') {
            let list = [...data]
            if(list.length > 0) {
                let detaile = list[0]
                bindHtml(detaile)
            }
        }
    })
}
getGoodDetail(goodID)

function bindHtml(detaile) {
    let detaileHtml = `
        <div class="show">
            <img src=${detaile.img} alt="">
        </div>
        <div class="info">
            <p class="title">${detaile.title}</p>
            <p class="price">¥ ${detaile.cPrice}</p>
            <button class="addBtn" goodID=${detaile.id}>加入购物车</button>
        </div>
    `
    $('div.content').html(detaileHtml)
} 

$('.content').on('click', 'button', function(e){
    e.stopPropagation();
    const user = JSON.parse(window.localStorage.getItem('user'))
    if(!user) {
        window.alert('尚未登录, 跳转登录页面')
        window.location.href = './login.html'
    } else {
        tryAddCartClick($(this).attr('goodID'), user.id)
    }
})

function tryAddCartClick(goodID, userID) {
    $.get(`http://localhost:3100/cards?userId=${userID}&goodId=${goodID}`, (data, status)=> {
        if(status == 'success') {
            let cartList = [...data]
            if(cartList.length > 0) {
                /// 修改商品数量
                let count = parseInt(cartList[0].count) + 1
                let cartID = cartList[0].id
                let info = {count}
                var param = JSON.stringify(info)
                var xhr3 = new XMLHttpRequest()
                xhr3.onload = function(event) {
                    window.alert('添加购物车成功')
                }
                xhr3.open('PATCH', `http://localhost:3100/cards/${cartID}`, true)
                xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
                xhr3.send(param)
            } else {
                /// 添加商品 count
                addCartClick(goodID, userID)
            }
        }
    })
}

function addCartClick(goodID, userID) {
    let info = {goodId: goodID, userId: userID, count: 1, isSelect:true}
    console.log('info:', info)
    $.post('http://localhost:3100/cards', info, (data, status)=> {
        if(status == 'success') {
            window.alert('添加购物车成功')
        }
    })
}

