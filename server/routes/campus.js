const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all campus */
router.get('/', async (req, res, next) => {
    try {
        const campus = await store.campus.get();
        res.json(campus);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new campus */
router.post('/', async (req, res, next) => {
    try {
        await store.campus.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random campus */
router.post('/random', async (req, res, next) => {
    try {
        const roles = await store.role.get();
        const values = {
            [C.CAMPUS.NAME]: faker.address.county(),
        };
        const item = await store.campus.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete campus by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.campus.delete(id);
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit campus by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.campus.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;