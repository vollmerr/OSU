import mysql from 'mysql';

const isProd = proccess.env.NODE_ENV === 'production';
const host = isProd ? proccess.env.HEROKU_URL : 'localhost';
const conn = mysql.createConnection({
    host,
    user: 'vollmerr',
    password: 'password...',
    database: 'cs340',
});

connection.connect();

connection.query('SELECT 1 + 1 AS solution', function (error, results, fields) {
    if (error) throw error;
    console.log('The solution is: ', results[0].solution);
});

connection.end();