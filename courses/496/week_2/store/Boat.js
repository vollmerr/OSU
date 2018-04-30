const uuid = require('uuid/v4');

const ds = require('./connect');


const schema = {
    id: 'id',
    name: 'name',
    type: 'type',
    length: 'length',
    atSea: 'at_sea',
};

const kind = 'Boat';


// add a new boat
const create = (boat) => {
    const id = uuid();
    const data = {
        ...boat,
        [schema.id]: id,
        [schema.atSea]: true,
    };
    // insert into db
    ds.insert({ data, key: ds.key([kind, id]) });
    // return the new record
    return data;
};

// delete a boat
const del = async (id) => {
    const key = ds.key([kind, id]);
    return ds.delete(key);
};

// edit a boat (apparently no way to partial update?)
const update = async (boat) => {
    const { id, ...updates } = boat;
    const [res] = await getById(id);
    const data = { ...res, ...boat };
    const key = ds.key([kind, id]);
    await ds.update({ key, data });
    return data;
};

// get all boats
const get = async () => {
    const q = ds.createQuery(kind);
    return ds.runQuery(q);
};

// get a boat by id
const getById = (id) => {
    const key = ds.key([kind, id]);
    return ds.get(key);
};


module.exports = Object.freeze({
    schema,
    kind,
    create,
    del,
    update,
    get,
    getById,
});