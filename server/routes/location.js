const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all location */
router.get('/', async (req, res, next) => {
    try {
        const locations = await store.location.get({});
        res.json(locations);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new location */
router.post('/', async (req, res, next) => {
    try {
        await store.location.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random location */
router.post('/random', async (req, res, next) => {
    try {
        const values = {
            [C.LOCATION.NAME]: faker.address.secondaryAddress(),
        };
        const item = await store.location.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete location by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.location.delete({ id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit location by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.location.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;