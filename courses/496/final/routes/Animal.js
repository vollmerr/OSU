const router = require('express').Router();

const validate = require('../middleware/validate');
const Animal = require('../store/Animal');

router.get('/', async (req, res) => {
  const data = await Animal.read();
  res.status(200).json(data);
});

router.get('/:id', async (req, res) => {
  const data = await Animal.readOne(req.params.id);
  res.status(200).json(data);
});

// TODO: validate valid animalId
router.post('/', validate(Animal.validation.create), async (req, res) => {
  const data = await Animal.create(req.body);
  res.status(201).json(data);
});

router.put('/:id', validate(Animal.validation.update), async (req, res) => {
  const data = await Animal.update(req.params.id, req.body);
  res.status(200).json(data);
});

router.delete('/:id', async (req, res) => {
  await Animal.delete(req.params.id);
  res.sendStatus(204);
});

module.exports = router;
