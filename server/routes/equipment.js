const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all equipment */
router.get('/', async (req, res, next) => {
    try {
        const items = await store.equipment.get();
        res.json(items);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new equipment */
router.post('/', async (req, res, next) => {
    try {
        await store.equipment.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random equipment */
router.post('/random', async (req, res, next) => {
    try {
        const values = {
            [C.EQUIPMENT.NAME]: faker.commerce.product(),
        };
        await store.equipment.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete equipment by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.equipment.delete(id);
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit equipment by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.equipment.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;