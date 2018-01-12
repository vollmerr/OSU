const router = require('express').Router();


/* GET - get home page. */
router.get('*', function(req, res, next) {
  res.sendFile(__dirname + '../../client/build/index.html');
});


module.exports = router;
