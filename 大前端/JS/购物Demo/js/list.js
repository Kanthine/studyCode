const info = {
    currentPage: 2,
    totalPage: 99,
    pageSize: 20,
    search: '',
    filter: 'all',
    discount: '10',
    saleType: 20,
    sortType: 'id',
    sortMethod: 'ASC',
    category: '全部',
}

// 1、请求分类列表，渲染分类
function getCateList() {
    $.get('http://localhost:3100/categories', (data, status)=> {
        if(status == 'success') {
            let kinds = [...data]
            let str = ``
            if(info.category == '全部') {
                str = `<li class="active">全部</li>`
            } else {
                str = `<li>全部</li>`
            }
            kinds.forEach(item=>{
                if(info.category == item.title) {
                    str += `<li class="active">${item.title}</li>`
                } else {
                    str += `<li>${item.title}</li>`
                }
            })
            $('.cate').html(str)
        }
    })
}
getCateList()



function getGoodsList() {
    $.get('http://localhost:3100/goods', info, (data, status)=> {
        if(status == 'success') {
            let list = [...data]
            bindHtml(list);
        }
    })
}
getGoodsList()

function bindHtml(list) {
    if(info.currentPage == 1) {
        $('.left').addClass('disbale')
    } else {
        $('.left').removeClass('disbale')
    }
    if(info.currentPage == info.totalPage) {
        $('.right').addClass('disbale')
    } else {
        $('.right').removeClass('disbale')
    }
    $('.pageInfo').text(`${info.currentPage} / ${info.totalPage}`)
    $('select').val(info.pageSize)
    $('.page').val(info.currentPage)

    let goodsHtml = ``
    list.forEach(item=>{
        goodsHtml += `
            <li goodID=${item.id}>
                <div class="show">
                    <img src=${item.img} alt="">
                    ${item.isHot ? '<div class="hot">hot</div>' : ''}
                    ${item.isSale ? '<div class="sale">sale</div>' : ''}
                </div>
                <div class="info">
                    <p class="title">${item.title}</p>
                    <p class="price">
                        <span class="current">¥ ${item.cPrice}</span>
                        <span class="old">¥ ${item.oPrice}</span>
                    </p>
                    <button goodID=${item.id}>加入购物车</button>
                </div>
            </li>
            `
        })
        $('div.list').html(goodsHtml)
} 

// 分类事件
$('.cate').on('click', 'li', function(){
    console.log('cate', this)
    $(this).addClass('active').siblings().removeClass('active');
    info.category = $(this).text() == '全部' ? '' : $(this).text();
    info.currentPage = 1
    getGoodsList()
})

// 筛选事件
$('.filter').on('click', 'li', function(){
    $(this).addClass('active').siblings().removeClass('active');
    info.filter = $(this).attr('type')
    info.currentPage = 1
    getGoodsList()
})

// 折扣事件
$('.discount').on('click', 'li', function(){
    $(this).addClass('active').siblings().removeClass('active');
    info.discount = $(this).attr('type')
    info.currentPage = 1
    getGoodsList()
})

// 模糊搜索
$('.search').on('input', function(){
    info.search = $(this).val().trim()
    info.currentPage = 1
    getGoodsList()
})

$('.left').on('click', function() {
    if(info.currentPage > 1) {
        info.currentPage --
        getGoodsList()
    }
})

$('.right').on('click', function(){
    if(info.currentPage < info.totalPage) {
        info.currentPage ++
        getGoodsList()
    }
})

$('select').on('change', function(){
    info.pageSize = $(this).val()
    info.currentPage = 1
    getGoodsList()
})

$('.jump').on('click',function(){
    let thePage = $('.page').val()
    if(thePage > info.totalPage) {
        thePage = info.totalPage
    } else if (thePage < 1) {
        thePage = 1
    }
    info.currentPage = thePage
    getGoodsList()
})

$('.list').on('click', 'li', function(e){
    window.localStorage.setItem('goodID', $(this).attr('goodID'))
    console.log('商品详情', $(this).attr('goodID'))
    window.location.href = './detaile.html'
})

$('.list').on('click', 'button', function(e){
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

