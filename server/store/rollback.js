const store = require('./index');


(async () => {
    try {
        await store.equipment.drop();
        await store.admin.drop();
        await store.role.drop();
        console.log('rollback completed');
    } catch (e) {
        console.error(e);
    } finally {
        store.con.end();
    }
})();
