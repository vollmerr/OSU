'use strict';

const getRequest = require(`@google-cloud/nodejs-repo-tools`).getRequest;
const test = require(`ava`);
const delay = require('delay');

const Boat = require('../store/Boat');
const Slip = require('../store/Slip');


const props = {
    number: 123,
};
const type = 'slip';


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
        [Slip.schema.number]: props.number,
    };
    // make resquest
    const res = await getRequest(testConfig)
        .post(`/${type}s`)
        .send(body);
    // ok response
    t.is(res.status, 200);
    // set gloabl id for other tests...
    id = res.body[Slip.schema.id];
    // id exists in uuid format
    t.regex(id, /^(\d|\S|-)*$/);
    // boat details are set to empty
    t.truthy(res.body[Slip.schema.currentBoat] === null);
    t.truthy(res.body[Slip.schema.arrivalDate] === null);
    t.truthy(res.body[Slip.schema.departureHistory].length === 0);
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
    // give time to process adding slip in previous test
    await delay(2000);
    // make request
    const res = await getRequest(testConfig)
        .get(`/${type}s`);
    // ok response
    t.is(res.status, 200);
    // returns an array
    t.truthy(Array.isArray(res.body));
    // objects in array are properly formatted slips
    Object.values(Slip.schema).forEach((x) => {
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
    t.truthy(res.body[Slip.schema.id] === id);
    // object is properly formatted slip
    Object.values(Slip.schema).forEach((x) => {
        t.truthy(res.body.hasOwnProperty(x));
    });
});


test.serial(`show the boat in a ${type} by id`, async (t) => {
    // create a boat
    const boat = await getRequest(testConfig)
        .post(`/boats`)
        .send({
            [Boat.schema.name]: 'boat name',
            [Boat.schema.type]: 'boat type',
            [Boat.schema.length]: 123,
        });
    // assign boat to slip
    await getRequest(testConfig)
        .put(`/${type}s/${id}`)
        .send({ [Slip.schema.currentBoat]: boat.body[Boat.schema.id] });
    // make request
    const res = await getRequest(testConfig)
        .get(`/${type}s/${id}/boat`);
    // ok response
    t.is(res.status, 200);
    // get boat
    const boatRes = await getRequest(testConfig)
        .get(`/boats/${boat.body[Boat.schema.id]}`);
    // the correct boat was returned
    t.deepEqual(res.body, boatRes.body)
});


test.serial(`edit a ${type} by id`, async (t) => {
    const body = {
        [Slip.schema.arrivalDate]: new Date().toISOString(),
        [Slip.schema.currentBoat]: '123',
        [Slip.schema.number]: 1,
    };
    // get slip before update
    const before = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // make request
    const res = await getRequest(testConfig)
        .put(`/${type}s/${id}`)
        .send(body);
    // ok response
    t.is(res.status, 200);
    // check slip has been updated
    const after = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // should be same as before, with new values updated
    t.deepEqual(after.body, { ...before.body, ...body });
});


test.serial(`delete a ${type} by id`, async (t) => {
    // create a boat
    const boat = await getRequest(testConfig)
        .post(`/boats`)
        .send({
            [Boat.schema.name]: 'boat name',
            [Boat.schema.type]: 'boat type',
            [Boat.schema.length]: 123,
        });
    // assign boat to slip
    await getRequest(testConfig)
        .put(`/${type}s/${id}`)
        .send({ [Slip.schema.currentBoat]: boat.body[Boat.schema.id] });
    // make request
    const res = await getRequest(testConfig)
        .delete(`/${type}s/${id}`);
    // no content returned
    t.is(res.status, 204);
    // check actually deleted 
    const newRes = await getRequest(testConfig)
        .get(`/${type}s/${id}`);
    // no body, as does not exist
    t.truthy(newRes.body === '');
    // check boat has been set to at sea
    const newBoat = await getRequest(testConfig)
        .get(`/boats/${boat.body[Boat.schema.id]}`);
    t.truthy(newBoat.body[Boat.schema.atSea] === true);
});


// clean up
test.after.always(async (t) => {
    if (id) {
        const res = await getRequest(testConfig)
            .delete(`/${type}s/${id}`);
        t.is(res.status, 204);
    }
});