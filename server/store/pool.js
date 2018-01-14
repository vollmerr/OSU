const mysql = require('mysql');

const connections = {
    development: {
        connectionLimit : 10,
        user: 'root',
        password: 'password',
        host: 'localhost',
        database: 'osu',
    },
    production: {
        connectionLimit : 10,
        user: 'bec70745933b8f',
        password: '888e8e74',
        host: 'us-cdbr-iron-east-05.cleardb.net',
        database: 'heroku_13d5a4c30c88dc5',
    }
};

const pool = mysql.createPool(connections[process.env.NODE_ENV]);

module.exports = pool;
