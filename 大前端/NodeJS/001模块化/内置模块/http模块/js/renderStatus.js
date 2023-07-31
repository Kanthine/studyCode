function renderStatus(url) {
    let arr = ['/home', '/index', '/json']
    return arr.includes(url) ? 200 : 404
}

module.exports.renderStatus = renderStatus;