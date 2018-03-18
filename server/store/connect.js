const mysql = require('mysql');


const connections = {
    development: {
        user: 'root',
        password: 'password',
        host: 'localhost',
        database: 'final',
    },
    production: {
        user: 'bec70745933b8f',
        password: '888e8e74',
        host: 'us-cdbr-iron-east-05.cleardb.net',
        database: 'heroku_13d5a4c30c88dc5',
    } 
};

const con = mysql.createConnection(connections[process.env.NODE_ENV || 'development']);


module.exports = con;
