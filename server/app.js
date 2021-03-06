const express = require('express');
const path = require('path');
const favicon = require('serve-favicon');
const logger = require('morgan');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');
const compression = require('compression')

const role = require('./routes/role');
const campus = require('./routes/campus');
const location = require('./routes/location');
const campusLocation = require('./routes/campusLocation');
const badgeType = require('./routes/badgeType');
const admin = require('./routes/admin');
const visit = require('./routes/visit');
const visitor = require('./routes/visitor');
const index = require('./routes/index');

const app = express();

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(compression())
app.use(express.static(path.join(__dirname, '../client/build')));

app.use('/role', role);
app.use('/campus', campus);
app.use('/location', location);
app.use('/campusLocation', campusLocation);
app.use('/badgeType', badgeType);
app.use('/admin', admin);
app.use('/visit', visit);
app.use('/visitor', visitor);
app.use('/', index);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  const err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  console.error(err.stack)
  res.status(500).send('Something broke!');
});

module.exports = app;
