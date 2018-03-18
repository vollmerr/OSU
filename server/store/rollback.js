const store = require('./index');


(async () => {
    try {
        await store.equipment.drop();
        await store.role.drop();
        await store.admin.drop();
        console.log('rollback completed');
    } catch (e) {
        console.error(e);
    } finally {
        store.con.end();
    }
})();
