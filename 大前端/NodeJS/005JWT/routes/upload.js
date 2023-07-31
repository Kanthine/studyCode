var express = require('express');
const JWT = require('../unit/JWT');
var router = express.Router();

router.get('/', function(req, res, next) {
  res.render('upload', { title: '文件上传' });
});

module.exports = router;