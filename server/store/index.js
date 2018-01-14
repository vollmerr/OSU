const con = require('./connect');


// calls database - handles opening and closing connection
const query = (query) => (
    new Promise((resolve, reject) => {
        return con.query(query, (err, result) => {  
            if (err) {
                return reject(err);
            }
            return resolve(result);
        });
    })
);

// create users table
const createTableUsers = () => (
    query(`create table users(
        id int primary key auto_increment,
        username varchar(255) not null,
        password varchar(255) not null        
    )`)
);

// drop users table
const dropTableUsers = () => (
    query('drop table if exists users')
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
    createTableUsers,
    dropTableUsers,
    getUsers,
    createUser,
    deleteUser,
};