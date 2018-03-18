const router = require('express').Router();
const store = require('../store');
const faker = require('faker');


/* GET - get all equipment */
router.get('/', async (req, res, next) => {
    try {
        const users = await store.equipment.getEquipment();
        res.json(users);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new equipment */
router.post('/', async (req, res, next) => {
    try {
        await store.equipment.createEquipment(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random equipment */
router.post('/random', async (req, res, next) => {
    try {
        const name = faker.commerce.product();
        const item = await store.equipment.createEquipment({ name });
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete equipment by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.equipment.deleteEquipment(id);
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
        await store.equipment.editEquipment({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;