const store = require('./index');

(async () => {
    await store.dropTableUsers();
    await store.createTableUsers();
})();