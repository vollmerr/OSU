const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all visitor */
router.get('/', async (req, res, next) => {
    try {
        const visitor = await store.visitor.get();
        res.json(visitor);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new visitor */
router.post('/', async (req, res, next) => {
    try {
        await store.visitor.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random visitor */
router.post('/random', async (req, res, next) => {
    try {
        // get list of available and in use
        const [badgeTypes, visits] = await Promise.all([
            store.badgeType.get({}),
            store.visit.get({}),
        ]);

        const values = {
            [C.VISITOR.FIRST_NAME]: faker.name.firstName(),
            [C.VISITOR.LAST_NAME]: faker.name.lastName(),
            [C.VISITOR.BADGE_ID]: faker.random.objectElement(badgeTypes)[C.BADGE_TYPE.ID],
            [C.VISITOR.VISIT_ID]: faker.random.objectElement(visits)[C.VISIT.ID],
        };
        await store.visitor.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete visitor by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.visitor.delete({ id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* PUT - edit visitor by id */
router.put('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        const body = req.body;
        await store.visitor.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;