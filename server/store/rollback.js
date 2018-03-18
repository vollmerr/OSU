const store = require('./index');


(async () => {
    try {
        const result = await store.equipment.dropTableEquipment();
        console.log('rollback completed', result);
    } catch (e) {
        console.error(e);
    }
})();
