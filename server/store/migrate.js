const store = require('./index');


(async () => {
    try {
        const result = await store.createTableUsers();
        console.log('migrations completed', result);
    } catch (e) {
        console.error(e);
    }
})();
