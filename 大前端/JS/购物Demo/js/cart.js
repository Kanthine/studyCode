let user = JSON.parse(window.localStorage.getItem('user'))
if (!user) {
    window.location.href('./login.html')
} 

function getCartList() {
    $.get(`http://localhost:3100/cards?userId=${user.id}&_expand=good`, (data, status)=> {
        if(status == 'success') {
            let cartList = [...data]
            console.log('cartList', cartList)
            
            bindHtml(cartList)
        }
    })
}
getCartList()

function bindHtml(list) {
    if (list.length < 1) {
        $('.empty').addClass('active')
        $('.list').removeClass('active')
        return
    } 
    $('.empty').removeClass('active')
    $('.list').addClass('active')
    
    let totalPrice = 0.0
    let totalNumber = 0
    let selNum = 0
    let goodsHtml = ``
    list.forEach(item=>{

        if(item.isSelect) {
            selNum += 1
            totalNumber += item.count
            totalPrice += (parseFloat(item.good.cPrice) * parseInt(item.count))
        }
        goodsHtml += `
            <li>
                <div class="select">
                    <input type="checkbox" cartID=${item.id} ${item.isSelect ? `checked` : ``}>
                </div>
                <div class="show">
                    <img src=${item.good.img}>
                </div>
                <div class="title">${item.good.title}</div>
                <div class="price">¥ ${item.good.cPrice}</div>
                <div class="number">
                    <button class='-Btn' cartID=${item.id} goodCount=${item.count}>-</button>
                    <input type="text" value=${item.count} disabled class="cart_number">
                    <button class='+Btn' cartID=${item.id} goodCount=${item.count}>+</button>
                </div>
                <div class="subPrice">¥ ${item.good.cPrice * item.count}</div>
                <div class="delete">
                    <button cartID=${item.id}>删除</button>
                </div>
            </li>
            `
    })

    console.log('totalPrice', totalPrice)

    let listHtml = `<div class="top">
                        全选 <input type="checkbox"  ${selNum == list.length ? `checked` : ``}>
                    </div>
                    <div class="center">
                        购物车列表
                        ${goodsHtml}
                    </div>
                    <div class="bottom">
                        <p>
                            共计 <span>${totalNumber}</span> 件商品
                        </p>
                        <div class="btns">
                            <button class="clearAll">清空购物车</button>
                            <button class="clearSel" ${selNum == 0 ? `disabled` : ``}>删除选中</button>
                            <button class="goPay" ${selNum == 0 ? `disabled` : ``}>去支付</button>
                        </div>
                        <p>
                            共计 ¥<span>${totalPrice.toFixed(2)}</span>
                        </p>
                    </div>`
    $('div.list').html(listHtml)
} 

$('.list').on('click', '.center .select input', function(){
    let val = ($(this).prop('checked'))
    let cartID = $(this).attr('cartID')    
    let info = {isSelect: val}
    var param = JSON.stringify(info)
    var xhr3 = new XMLHttpRequest()
    xhr3.onload = function(event) {
        getCartList()
    }
    xhr3.open('PATCH', `http://localhost:3100/cards/${cartID}`, true)
    xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
    xhr3.send(param)
})

$('.list').on('click', '.center .number button', function(){
    let calssName = $(this).attr('class')
    let isAdd = calssName == '+Btn' ? true : false
    let cartID = $(this).attr('cartID')
    let goodCount = parseInt($(this).attr('goodCount'))
    if (isAdd) {
        /// $(this).prev().val()
        goodCount += 1
    } else {
        /// $(this).next().val()
        goodCount -= 1
    }

    if (goodCount < 1) {
        /// 删除
        var xhr3 = new XMLHttpRequest()
        xhr3.onload = function(event) {
            getCartList()
        }
        xhr3.open('DELETE', `http://localhost:3100/cards/${cartID}`, true)
        xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
        xhr3.send()
        return
    }

    let info = {count: goodCount}
    var param = JSON.stringify(info)
    var xhr3 = new XMLHttpRequest()
    xhr3.onload = function(event) {
        getCartList()
    }
    xhr3.open('PATCH', `http://localhost:3100/cards/${cartID}`, true)
    xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
    xhr3.send(param)
})


$('.list').on('click', '.center .delete button', function(){
    let cartID = $(this).attr('cartID')
    var xhr3 = new XMLHttpRequest()
    xhr3.onload = function(event) {
        getCartList()
    }
    xhr3.open('DELETE', `http://localhost:3100/cards/${cartID}`, true)
    xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
    xhr3.send()
})

/// 批量修改
$('.list').on('click', '.top input', function(){
    let isAll = $(this).prop('checked')
    console.log('全选', isAll)

    // const user = JSON.parse(window.localStorage.getItem('user'))

    let info = {isSelect: isAll}
    var param = JSON.stringify(info)
    var xhr3 = new XMLHttpRequest()
    xhr3.onload = function(event) {
        getCartList()
    }
    xhr3.open('PUT', `http://localhost:3100/cards/userId=${user.id}`, true)
    xhr3.setRequestHeader("Content-Type", "application/json;charset=UTF-8")
    xhr3.send(param)
})

$('.list').on('click', '.bottom .btns .clearAll', function(){
    console.log('清空购物车')
})

$('.list').on('click', '.bottom .btns .clearSel', function(){
    console.log('清空选中')
})

$('.list').on('click', '.bottom .btns .goPay', function(){
    console.log('去支付')
})
