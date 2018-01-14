const router = require('express').Router();
const store = require('../store');
const faker = require('faker');


/* GET - get all users */
router.get('/', async (req, res, next) => {
  try {
    const users = await store.getUsers();
    res.json(users);
  } catch (err) {
    res.status(500).json(err);
  }
});


/* POST - create new random user */
router.post('/create', async (req, res, next) => {
  try {
    const user = {
      username: faker.internet.userName(),
      password: faker.internet.password(),
    };

    await store.createUser(user);
    res.sendStatus(200);
  } catch (err) {
    res.status(500).json(err);
  }
});


/* POST - delete user by id */
router.post('/:id/delete', async (req, res, next) => {
  try {
    const id = req.params.id;
    await store.deleteUser(id);
    res.sendStatus(204);
  } catch (err) {
    res.status(500).json(err);
  }
});


module.exports = router;