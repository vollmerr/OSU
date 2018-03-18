const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all campusLocation */
router.get('/', async (req, res, next) => {
    try {
        const campusLocation = await store.campusLocation.get({});
        res.json(campusLocation);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new campusLocation */
router.post('/', async (req, res, next) => {
    try {
        await store.campusLocation.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random campusLocation */
router.post('/random', async (req, res, next) => {
    try {
        // get list of available and in use
        const [locations, campus] = await Promise.all([
            store.location.get({}),
            store.campus.get({}),
        ]);

        const values = {
            [C.CAMPUS_LOCATION.CAMPUS_ID]: faker.random.objectElement(campus)[C.CAMPUS.ID],
            [C.CAMPUS_LOCATION.LOCATION_ID]: faker.random.objectElement(locations)[C.LOCATION.ID],
        };
        await store.campusLocation.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete campusLocation by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.campusLocation.delete({ id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit campusLocation by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.campusLocation.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;