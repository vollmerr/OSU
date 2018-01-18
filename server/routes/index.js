const router = require('express').Router();


/* GET - get home page. */
router.get('*', function(req, res, next) {
  console.log('in get...')
  res.sendFile(__dirname + '../../client/build/index.html');
});


module.exports = router;
