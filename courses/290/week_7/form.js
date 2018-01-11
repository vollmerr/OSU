var express = require('express');

var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});
var bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 8080);

app.get('/',function(req,res){
  var query = [], context = {}, i;
  for (i in req.query){
    query.push({'key': i,'value':req.query[i]});
  }
  context.query = query;
  res.render('get', context);
});

app.post('/', function(req,res){
  var query = [], body = [], context = {}, i;
  for (i in req.query){
    query.push({'key': i,'value':req.query[i]});
  }
  for (i in req.body){
    body.push({'key': i,'value':req.body[i]});
  }
  context.query = query;
  context.body = body;
  res.render('post', context);
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
