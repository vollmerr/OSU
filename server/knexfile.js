const url = process.env.NODE_ENV === 'development' ?
  'osu' :
  'mysql://bec70745933b8f:888e8e74@us-cdbr-iron-east-05.cleardb.net/heroku_13d5a4c30c88dc5?reconnect=true';

module.exports = {
  client: 'mysql',
  connection: {
    user: 'root',
    password: 'password',
    database: url,
  },
};