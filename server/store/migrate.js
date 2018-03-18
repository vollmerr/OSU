const store = require('./index');


(async () => {
    try {
        const result = await store.equipment.createTableEquipment();
        console.log('migrations completed', result);
    } catch (e) {
        console.error(e);
    } finally {
        store.con.end();
    }
})();
