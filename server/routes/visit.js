const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all visit */
router.get('/', async (req, res, next) => {
    try {
        const visit = await store.visit.get();
        res.json(visit);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new visit */
router.post('/', async (req, res, next) => {
    try {
        if (req.body[C.VISIT.DATE]) {
            req.body[C.VISIT.DATE] = new Date(req.body[C.VISIT.DATE]).toLocaleString();
        }
        await store.visit.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random visit */
router.post('/random', async (req, res, next) => {
    try {
        // get list of campusLocaitons
        const campusLocations = await store.campusLocation.get({});
        const values = {
            [C.VISIT.DATE]: new Date(faker.date.future()).toLocaleString(),
            [C.VISIT.CAMPUS_LOCATION_ID]: faker.random.objectElement(campusLocations)[C.CAMPUS_LOCATION.ID],
        };
        await store.visit.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete visit by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.visit.delete({ id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit visit by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        if (req.body[C.VISIT.DATE]) {
            req.body[C.VISIT.DATE] = new Date(req.body[C.VISIT.DATE]).toLocaleString();
        }
        await store.visit.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;