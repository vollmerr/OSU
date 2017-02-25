var express = require('express');
var mysql = require('./dbcon.js');
var app = express();
var handlebars = require('express-handlebars').create({
    defaultLayout: 'main'
});
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({
    extended: false
}));
app.use(bodyParser.json());
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 8084);
app.use('/public', express.static('public'));
app.get('/', (req, res, next) => {
    var context = {};
    mysql.pool.query('SELECT * FROM workouts', (err, rows, fields) => {
        if (err) {
            next(err);
            return;
        }
        context.results = rows;
        res.render('home', context);
    });
});
app.post('/byId', (req, res) => {
    mysql.pool.query('SELECT * FROM workouts WHERE id=?', [req.body.id], (err, rows, fields) => {
        if (err) {
            res.sendStatus(500);
        }
        else {
            res.json(rows);
        }
    });
});
app.post('/add', (req, res) => {
    if (req.body.name) {
        mysql.pool.query("INSERT INTO workouts VALUES ('', ?, ?, ?, ?, ?)", [
            req.body.name,
            req.body.reps !== 'undefined' ? req.body.reps : 0,
            req.body.weight !== 'undefined' ? req.body.weight : 0,
            req.body.date || new Date().toISOString().slice(0,10),
            req.body.lbs !== 'undefined' ? req.body.lbs : 0
        ], (err, result) => {
            res.json(result);
        });
    }
});
app.post('/delete', (req, res) => {
    mysql.pool.query("DELETE FROM workouts WHERE id=?", [req.body.id], (err, result) => {
        res.sendStatus(err ? 500 : 200);
    });
});
app.post('/edit', (req, res) => {
    mysql.pool.query("SELECT * FROM workouts WHERE id=?", [req.body.id], (err, result) => {
        if (err) {
            res.sendStatus(500);
        }
        if (result.length) {
            let curVals = result[0];
            mysql.pool.query("UPDATE workouts SET name=?, reps=?, weight=?, date=?, lbs=? WHERE id=?", [
                req.body.name !== 'undefined' ? req.body.name : curVals.name,
                req.body.reps !== 'undefined' ? req.body.reps : curVals.reps,
                req.body.weight !== 'undefined' ? req.body.weight : curVals.weight,
                req.body.date !== 'undefined' ? req.body.date : curVals.date,
                req.body.lbs !== 'undefined' ? req.body.lbs : curVals.lbs,
                req.body.id
            ], (err, result) => {
                res.sendStatus(err ? 500 : 200);
            });
        }
    });
});
app.get('/reset-table', (req, res, next) => {
    let context = {};
    mysql.pool.query("DROP TABLE IF EXISTS workouts", function (err) {
        let createString = "CREATE TABLE workouts(" + "id INT PRIMARY KEY AUTO_INCREMENT," + "name VARCHAR(255) NOT NULL," + "reps INT," + "weight INT," + "date DATE," + "lbs BOOLEAN)";
        mysql.pool.query(createString, function (err) {
            context.results = "Table reset";
            res.render('home', context);
        })
    });
});
app.use((req, res) => {
    res.status(404);
    res.render('404');
});
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(500);
    res.render('500');
});
app.listen(app.get('port'), () => {
    console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});
