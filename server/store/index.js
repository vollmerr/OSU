const knex = require('knex')(require('./knexfile'));


// gets all users
const getUsers = () => (
    knex('user').select()
);

// creates a new user
const createUser = ({ username, password }) => (
    knex('user').insert({ username, password })
);

// deletes a user by id
const deleteUser = (id) => (
    knex('user')
        .where('id', id)
        .del()
);


module.exports = {
    getUsers,
    createUser,
    deleteUser,
};