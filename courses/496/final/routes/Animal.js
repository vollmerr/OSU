const router = require('express').Router();

const validate = require('../middleware/validate');
const Animal = require('../store/Animal');
const Visit = require('../store/Visit');

//  view all animals
router.get('/', async (req, res) => {
  const animals = await Animal.read(req.user.id);

  if (animals) {
    return res.status(200).json(animals);
  }

  return res.sendStatus(404);
});

// view a animal by id
router.get('/:id', async (req, res) => {
  const animal = await Animal.readOne(req.user.id, req.params.id);

  if (animal) {
    return res.status(200).json(animal);
  }

  return res.sendStatus(404);
});

// see all visits for animal
router.get('/:id/visits', async (req, res) => {
  const visits = await Visit.findByAnimalId(req.user.id, req.params.id);

  if (visits) {
    return res.status(200).json(visits);
  }

  return res.sendStatus(404);
});

// see visit by id for animal
router.get('/:id/visits/:visitId', async (req, res) => {
  const visit = await Visit.readOne(req.user.id, req.params.visitId);

  if (visit) {
    return res.status(200).json(visit);
  }

  return res.sendStatus(404);
});

// create a new animal
router.post('/', validate(Animal.validation.create), async (req, res) => {
  const animal = await Animal.create(req.user.id, req.body);
  res.status(201).json(animal);
});

// edit a animal
router.put('/:id', validate(Animal.validation.update), async (req, res) => {
  const animal = await Animal.update(req.user.id, req.params.id, req.body);

  if (animal) {
    return res.status(200).json(animal);
  }

  return res.sendStatus(404);
});

// delete a animal
router.delete('/:id', async (req, res) => {
  const animalId = req.params.id;

  await Animal.delete(req.user.id, animalId);
  await Visit.deleteByAnimalId(req.user.id, animalId);

  res.sendStatus(204);
});

module.exports = router;
