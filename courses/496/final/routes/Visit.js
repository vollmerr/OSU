const router = require('express').Router();

const validate = require('../middleware/validate');
const Visit = require('../store/Visit');

router.get('/', async (req, res) => {
  const data = await Visit.read();
  res.status(200).json(data);
});

router.get('/:id', async (req, res) => {
  const data = await Visit.readOne(req.params.id);
  res.status(200).json(data);
});

// TODO: validate valid animalId
router.post('/', validate(Visit.validation.create), async (req, res) => {
  const data = await Visit.create(req.body);
  res.status(201).json(data);
});

router.put('/:id', validate(Visit.validation.update), async (req, res) => {
  const data = await Visit.update(req.params.id, req.body);
  res.status(200).json(data);
});

router.delete('/:id', async (req, res) => {
  await Visit.delete(req.params.id);
  res.sendStatus(204);
});

module.exports = router;
