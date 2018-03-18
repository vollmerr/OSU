const router = require('express').Router();
const store = require('../store');
const faker = require('faker');

const C = require('../store/constants');


/* GET - get all badgeType */
router.get('/', async (req, res, next) => {
    try {
        const roles = await store.badgeType.get();
        res.json(roles);
    } catch (err) {
        res.status(500).json(err);
    }
});


/* POST - create new badgeType */
router.post('/', async (req, res, next) => {
    try {
        await store.badgeType.insert(req.body);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* POST - create new random badgeType */
router.post('/random', async (req, res, next) => {
    try {
        console.log('about to get vals')
        const values = {
            [C.BADGE_TYPE.TYPE]: faker.lorem.word(),
        };
        console.log('intering ,', values)
        await store.badgeType.insert(values);
        res.sendStatus(200);
    } catch (err) {
        res.status(500).json(err);
    }
});

/* DELETE - delete role by id */
router.delete('/:id', async (req, res, next) => {
    try {
        const id = req.params.id;
        await store.badgeType.delete(id);
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
        await store.badgeType.edit({ ...body, id });
        res.sendStatus(204);
    } catch (err) {
        res.status(500).json(err);
    }
});


module.exports = router;