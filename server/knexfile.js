const connections = {
  development: {
    client: 'mysql',
    connection: {
      user: 'root',
      password: 'password',
      host: 'localhost',
      database: 'osu',
    }
  },
  production: {
    client: 'mysql',
    connection: {
      user: 'bec70745933b8f',
      password: '888e8e74',
      host: 'us-cdbr-iron-east-05.cleardb.net',
      database: 'heroku_13d5a4c30c88dc5',
    }
  }
};

module.exports = connections[process.env.NODE_ENV];