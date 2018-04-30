const Datastore = require('@google-cloud/datastore');

const config = require('../config');


module.exports = Datastore({
    projectId: config.get('GCLOUD_PROJECT'),
});