const morgan = require('morgan');
const express = require('express');
const config = require('./config');
const bodyParser = require('body-parser');
const session = require('express-session');
const MemcachedStore = require('connect-memjs')(session);
const passport = require('passport');

const auth = require('./utils/auth');

const app = express();

app.disable('etag');
app.set('trust proxy', true);

// [START session]
// Configure the session and session storage.
const sessionConfig = {
  resave: false,
  saveUninitialized: false,
  secret: config.get('SECRET'),
  signed: true,
};

// In production use the App Engine Memcache instance to store session data,
// otherwise fallback to the default MemoryStore in development.
if (config.get('NODE_ENV') === 'production' && config.get('MEMCACHE_URL')) {
  sessionConfig.store = new MemcachedStore({
    servers: [config.get('MEMCACHE_URL')],
  });
}

app.use(session(sessionConfig));
// [END session]

// OAuth2
app.use(passport.initialize());
app.use(passport.session());
app.use(auth.router); // eslint-disable-line

// parsing requests
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// log all requests
app.use(morgan('common'));

// Routes
app.use('/animals', auth.required, require('./routes/Animal'));
app.use('/visits', auth.required, require('./routes/Visit'));

// health check / noop route
app.get('/status', (req, res) => {
  res.sendStatus(200);
});

// Redirect root to /animals
app.get('/', (req, res) => {
  res.redirect('/animals');
});

// Basic 404 handler
app.use((req, res) => {
  res.status(404).send('Not Found');
});

// Basic error handler
app.use((err, req, res) => {
  res.status(500).send(err.response || 'Something broke!');
});

if (module === require.main) {
  // Start the server
  const server = app.listen(config.get('PORT'), () => {
    const { port } = server.address();
    console.log(`App listening on port ${port}`); // eslint-disable-line
  });
}

module.exports = app;
