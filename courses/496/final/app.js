const morgan = require('morgan');
const express = require('express');
const config = require('./config');
const bodyParser = require('body-parser');

const app = express();

app.disable('etag');
app.set('trust proxy', true);

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(morgan('common'));

// Routes
app.use('/animals', require('./routes/Animal'));
app.use('/visits', require('./routes/Visit'));

// health check / noop route
app.get('/status', (req, res) => {
  res.sendStatus(200);
})

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
    console.log(`App listening on port ${port}`);
  });
}

module.exports = app;
