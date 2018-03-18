const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all role */
router.get('/', async (req, res, next) => {
    try {
        const roles = await store.role.get({});
        res.json(roles);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new role */
router.post('/', async (req, res, next) => {
    try {
        await store.role.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random role */
router.post('/random', async (req, res, next) => {
    try {
        const values = {
            [C.ROLE.NAME]: faker.name.jobDescriptor(),
        };
        await store.role.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete role by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.role.delete({ id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit role by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.role.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;