const mysql = require('mysql');

const connections = {
    development: {
        user: 'root',
        password: 'password',
        host: 'localhost',
        database: 'osu',
    },
    production: {
        user: 'bec70745933b8f',
        password: '888e8e74',
        host: 'us-cdbr-iron-east-05.cleardb.net',
        database: 'heroku_13d5a4c30c88dc5',
    }
};

// calls database - handles opening and closing connection
const query = (query) => (
    new Promise((resolve, reject) => {
        const con = mysql.createConnection(connections[process.env.NODE_ENV]);
        con.connect();
        return con.query(query, (err, result) => (
            err ? reject(err) : resolve(result)
        ));
    })
);

// get all users
const getUsers = () => (
    query('select * from users')
);  

// create a new user
const createUser = ({ username, password }) => (
    query(`insert into users (username, password) values ("${username}", "${password}")`)
);

// deletes a user by id
const deleteUser = (id) => (
    query(`delete from users where id=${id}`)
);


module.exports = {
    getUsers,
    createUser,
    deleteUser,
};