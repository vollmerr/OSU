const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all admin */
router.get('/', async (req, res, next) => {
    try {
        const admins = await store.admin.get();
        res.json(admins);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new admin */
router.post('/', async (req, res, next) => {
    try {
        await store.admin.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random admin */
router.post('/random', async (req, res, next) => {
    try {
        const roles = await store.role.get();
        const values = {
            [C.ADMIN.FIRST_NAME]: faker.name.firstName(),
            [C.ADMIN.LAST_NAME]: faker.name.lastName(),
            [C.ADMIN.ROLE_ID]: faker.random.objectElement(roles)[C.ROLE.ID],
        };
        const item = await store.admin.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete admin by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.admin.delete(id);
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit admin by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.admin.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;