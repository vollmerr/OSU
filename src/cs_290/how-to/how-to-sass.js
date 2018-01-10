var express = require('express');
var path = require('path');

var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 8081);

app.use('/public', express.static(path.join(__dirname, 'public')));

app.get('/',function(req,res){
  res.render('intro');
});

app.get('/setup',function(req,res){
  res.render('setup');
});

app.get('/sass',function(req,res){
  res.render('sass');
});

app.get('/custom',function(req,res){
  res.render('custom');
});

app.get('/more',function(req,res){
  res.render('more');
});

app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.type('plain/text');
  res.status(500);
  res.render('500');
});


app.listen(app.get('port'), function(){
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});
