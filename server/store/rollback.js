const store = require('./index');


try {
    const { con, ...rest } = store;
    Object.keys(rest).reverse().forEach(async (api) => {
        await store[api].drop();
    });
    console.log('rollback completed');
} catch (e) {
    console.error(e);
} finally {
    store.pool.end();
}