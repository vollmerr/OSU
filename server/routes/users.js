const router = require('express').Router();
const store = require('../store');
const faker = require('faker');


/* GET - get all users */
router.get('/', async (req, res, next) => {
  const users = await store.getUsers();
  res.json(users);
});


/* POST - create new random user */
router.post('/create', async (req, res, next) => {
  const user = {
    username: faker.internet.userName(),
    password: faker.internet.password(),
  };

  await store.createUser(user);
  res.sendStatus(200);
});


/* POST - delete user by id */
router.post('/:id/delete', async (req, res, next) => {
  const id = req.params.id;
  
  await store.deleteUser(id);
  res.sendStatus(204);
});


module.exports = router;