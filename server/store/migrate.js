const store = require('./index');


try {
    const { con, ...rest } = store;
    Object.keys(rest).forEach(async (api) => {
        await store[api].create();
    });
    console.log('migrations completed');
} catch (e) {
    console.error(e);
} finally {
    store.con.end();
}
