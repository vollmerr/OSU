const fetch = require('node-fetch');
const express = require('express');
const router = express.Router();

const Auth = require('../auth')();


router.get('/', (req, res, next) => {
  const oAuthUrl = Auth.getAuthUrl();
  res.render('index', { oAuthUrl });
});

router.get('/redirect', async (req, res, next) => {
  const { state, code, error } = req.query;
  // handle error
  if (error) { return res.render('error', { message: 'Oh Noes!', error }); }
  // handle invalid state
  if (!Auth.getState(state)) { return res.render('error', { message: 'Invalid Authorization State', error: {} }); }
  // get access token
  const { access_token, id_token, expires_in } = await Auth.getToken({ code });
  // set expiration based off current time and expires_in
  const exp = new Date().getTime() + (expires_in * 1000);
  res.render('redirect', { access_token, id_token, exp });
});

router.get('/404', (req, res, next) => {
  const message = 'Something went wrong :(';
  const error = req.query.error;
  res.render('error', { message, error });
});


module.exports = router;
