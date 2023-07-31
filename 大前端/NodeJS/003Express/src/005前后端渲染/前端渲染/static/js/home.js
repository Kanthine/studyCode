fetch(`/home/list`)
.then(res=>res.json())
.then(res=>{
    console.log('res' ,res)
    let arr = [...res]
    if (arr.length > 0) {
        let str = ``
        arr.forEach(item=>{
            str += `<li>${item.name}</li>`
        })
        $('.list').html(str)
    }
})