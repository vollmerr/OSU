const uuid = require('uuid/v4');

const ds = require('./connect');
const Boat = require('./Boat');

const schema = {
    id: 'id',
    number: 'number',
    currentBoat: 'current_boat',
    arrivalDate: 'arrival_date',
    departureHistory: 'departure_history',
};

const departureHistory = {
    departureDate: 'departure_date',
    departedBoat: 'departed_boat',
};

const kind = 'Slip';


// add a new slip
const create = (slip) => {
    const id = uuid();
    const data = {
        ...slip,
        [schema.id]: id,
        [schema.currentBoat]: null,
        [schema.arrivalDate]: null,
        [schema.departureHistory]: [],
    };
    // insert into db
    ds.insert({ data, key: ds.key([kind, id]) });
    // return the new record
    return data;
};

// delete a slip
const del = async (id) => {
    const key = ds.key([kind, id]);
    return ds.delete(key);
};

// edit a slip
const update = async (slip) => {
    const { id, ...updates } = slip;
    const [res] = await getById(id);
    const data = { ...res, ...slip };
    // if trying to assign a boat
    if (slip[schema.currentBoat]) {
        // if already occupied
        if (res[schema.currentBoat]) {
            return null;
        }
        // else assign to slip
        data[schema.currentBoat] = slip[schema.currentBoat];
        data[schema.arrivalDate] = new Date().toISOString();
    }
    const key = ds.key([kind, id]);
    await ds.update({ key, data });
    return data;
};

// get all slips
const get = () => {
    const q = ds.createQuery(kind);
    return ds.runQuery(q);
};

// get a slip by id
const getById = (id) => {
    const key = ds.key([kind, id]);
    return ds.get(key);
};

// adding boat to a slip
const assignBoat = async (id, boatId) => {
    // get the slip
    const [slip] = await getById(id);
    // if can assign
    if (slip && !slip[schema.currentBoat]) {
        // assign to slip
        const newSlip = {
            ...slip,
            [schema.currentBoat]: boatId,
            [schema.arrivalDate]: new Date().toISOString(),
        };
        await update(newSlip);
        return newSlip;
    } else {
        return null;
    }
};

// empty the slip... id=boat id
const empty = async (id) => {
    // find if boat is assigned to slip
    const q = ds.createQuery(kind)
        .filter(schema.currentBoat, '=', id);
    const [[slip]] = await ds.runQuery(q);
    if (slip) {
        // add boat to slips history
        const history = {
            [departureHistory.departedBoat]: id,
            [departureHistory.departureDate]: new Date().toISOString(),
        };
        const updates = {
            ...slip,
            [schema.currentBoat]: null,
            [schema.arrivalDate]: null,
            [schema.departureHistory]: [...slip[schema.departureHistory], history],
        };
        await update(updates);
    }
}


module.exports = Object.freeze({
    schema,
    departureHistory,
    kind,
    create,
    del,
    update,
    get,
    getById,
    assignBoat,
    empty,
});