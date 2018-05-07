const Datastore = require('@google-cloud/datastore');


module.exports = Datastore({
    projectId: process.env.GCLOUD_PROJECT,
});