'use strict';

const getRequest = require(`@google-cloud/nodejs-repo-tools`).getRequest;
const test = require(`ava`);
const delay = require('delay');

const Boat = require('../store/Boat');
const Slip = require('../store/Slip');


const props = {
    name: 'test name',
    type: 'test type',
    length: 123,
};
const type = 'boat';

let originalDataBackend, id, testConfig, appConfig;

test.before(() => {
    testConfig = require(`./config`);
    appConfig = require(`../config`);
    originalDataBackend = appConfig.get(`DATA_BACKEND`);
    appConfig.set(`DATA_BACKEND`, `datastore`);
});


test.serial(`add a new ${type}`, async (t) => {
    // all fields filled out correct
    const body = {
        [Boat.schema.name]: props.name,
        [Boat.schema.type]: props.type,
        [Boat.schema.length]: props.length,
    };
    // make resquest
    const res = await getRequest(testConfig)
        .post(`/${type}s`)
        .send(body);
    // ok response
    t.is(res.status, 200);
    // set gloabl id for other tests...
    id = res.body[Boat.schema.id];
    // id exists in uuid format
    t.regex(id, /^(\d|\S|-)*$/);
    // set to being at sea
    t.truthy(res.body[Boat.schema.atSea]);
    // values passed are set
    Object.keys(body).forEach((x) => {
        t.is(res.body[x], body[x]);
    });
});


test.serial(`validation when adding a new ${type}`, async (t) => {
    // send with missing fields
    const body = {};
    // make resquest
    const res = await getRequest(testConfig)
        .post(`/${type}s`)
        .send(body);
    // bad request
    t.is(res.status, 400);
    // has a validation error
    t.true(res.body.name === 'ValidationError');
});


test.serial(`show a list of ${type}s`, async (t) => {
    // give time to process adding boat in previous test
    await delay(2000);
    // make request
    const res = await getRequest(testConfig)
        .get(`/${type}s`);
    // ok response
    t.is(res.status, 200);
    // returns an array
    t.truthy(Array.isArray(res.body));
    // objects in array are properly formatted boats
    Object.values(Boat.schema).forEach((x) => {
        t.truthy(res.body[0].hasOwnProperty(x));
    });
});


test.serial(`show a ${type} by id`, async (t) => {
    // make request
    const res = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // ok response
    t.is(res.status, 200);
    // object has correct id
    t.truthy(res.body[Boat.schema.id] === id);
    // object is properly formatted boat
    Object.values(Boat.schema).forEach((x) => {
        t.truthy(res.body.hasOwnProperty(x));
    });
});


test.serial(`edit a ${type} by id`, async (t) => {
    const body = {
        [Boat.schema.atSea]: true,
        [Boat.schema.length]: 999,
        [Boat.schema.type]: 'new type',
        [Boat.schema.name]: 'new name',
    };
    // create a slip
    const slip = await getRequest(testConfig)
        .post(`/slips`)
        .send({ [Slip.schema.number]: 1 });
    // assign boat to slip
    await getRequest(testConfig)
        .put(`/slips/${slip.body[Slip.schema.id]}`)
        .send({ [Slip.schema.currentBoat]: id });
    // get boat before update
    const before = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // make request
    const res = await getRequest(testConfig)
        .put(`/${type}s/${id}`)
        .send(body);
    // ok response
    t.is(res.status, 200);
    // check boat has been updated
    const after = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // should be same as before, with new values updated
    t.deepEqual(after.body, { ...before.body, ...body });
    // check slip updated
    const newSlip = await getRequest(testConfig)
        .get(`/slips/${slip.body[Slip.schema.id]}`)
    // check all values emtpied
    t.truthy(newSlip.body[Slip.schema.currentBoat] === null);
    t.truthy(newSlip.body[Slip.schema.arrivalDate] === null);
    // check boat is added to history
    const departed = newSlip.body[Slip.schema.departureHistory][0][Slip.departureHistory.departedBoat];
    t.truthy(departed === id);
});


test.serial(`delete a ${type} by id`, async (t) => {
    // create a slip
    const slip = await getRequest(testConfig)
        .post(`/slips`)
        .send({ [Slip.schema.number]: 1 });
    // assign boat to slip
    await getRequest(testConfig)
        .put(`/slips/${slip.body[Slip.schema.id]}`)
        .send({ [Slip.schema.currentBoat]: id });
    // make request to delete
    const res = await getRequest(testConfig)
        .delete(`/${type}s/${id}`);
    // no content returned
    t.is(res.status, 204);
    // check actually deleted 
    const newRes = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // no body, as does not exist
    t.truthy(newRes.body === '');
    // get new slip
    const newSlip = await getRequest(testConfig)
        .get(`/slips/${slip.body[Slip.schema.id]}`);
    // check all values emtpied
    t.truthy(newSlip.body[Slip.schema.currentBoat] === null);
    t.truthy(newSlip.body[Slip.schema.arrivalDate] === null);
    // check boat is added to history
    const departed = newSlip.body[Slip.schema.departureHistory][0][Slip.departureHistory.departedBoat];
    t.truthy(departed === id);
});


// clean up
test.after.always(async (t) => {
    if (id) {
        const res = await getRequest(testConfig)
            .delete(`/${type}s/${id}`);
        t.is(res.status, 204);
    }
});