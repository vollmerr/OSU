// Hierarchical node.js configuration with command-line arguments, environment
// variables, and files.
const nconf = require('nconf');
const path = require('path');

module.exports = nconf
  // 1. Command-line arguments
  .argv()
  // 2. Environment variables
  .env([
    'CLOUD_BUCKET',
    'DATA_BACKEND',
    'GCLOUD_PROJECT',
    'MEMCACHE_URL',
    'INSTANCE_CONNECTION_NAME',
    'NODE_ENV',
    'OAUTH2_CLIENT_ID',
    'OAUTH2_CLIENT_SECRET',
    'OAUTH2_CALLBACK',
    'PORT',
    'SECRET',
  ])
  // 3. Config file
  .file({ file: path.join(__dirname, 'config.json') })
  // 4. Defaults
  .defaults({
    DATA_BACKEND: 'datastore',
    GCLOUD_PROJECT: 'vollmerr-final',
    MEMCACHE_URL: 'localhost:11211',
    OAUTH2_CLIENT_ID: '',
    OAUTH2_CLIENT_SECRET: '',
    OAUTH2_CALLBACK: 'http://localhost:8080/auth/google/callback',
    PORT: 8080,
    APP_URL: 'localhost',
    SECRET: 'superDuperSecret',
  });

function checkConfig(setting) {
  if (!nconf.get(setting)) {
    throw new Error(
      `You must set ${setting} as an environment variable or in config.json!`
    );
  }
}

// Check for required settings
checkConfig('GCLOUD_PROJECT');
checkConfig('OAUTH2_CLIENT_ID');
checkConfig('OAUTH2_CLIENT_SECRET');
