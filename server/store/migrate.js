const store = require('./index');


(async () => {
    try {
        await store.equipment.create();
        await store.role.create();
        await store.admin.create();
        console.log('migrations completed');
    } catch (e) {
        console.error(e);
    } finally {
        store.con.end();
    }
})();
