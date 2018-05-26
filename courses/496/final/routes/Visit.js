const router = require('express').Router();

const validate = require('../middleware/validate');
const Visit = require('../store/Visit');
const Animal = require('../store/Animal');

// get all visits
router.get('/', async (req, res) => {
  const visits = await Visit.read();
  res.status(200).json(visits);
});

// get a visit by id
router.get('/:id', async (req, res) => {
  const visit = await Visit.readOne(req.params.id);

  if (visit) {
    return res.status(200).json(visit);
  }

  return res.sendStatus(404);
});

// get an visits animal
router.get('/:id/animal', async (req, res) => {
  const visit = await Visit.readOne(req.params.id);

  if (visit) {
    const animal = await Animal.readOne(visit.id);

    if (animal) {
      return res.status(200).json(animal);
    }
  }

  return res.sendStatus(404);
});

// create a new visit
router.post('/', validate(Visit.validation.create), async (req, res) => {
  const animal = await Animal.readOne(req.body.animalId);

  if (animal) {
    const visit = await Visit.create(req.body);
    // add visit to the animals visits
    await Animal.addVisit(animal.id, visit.id);
    return res.status(201).json(visit);
  }

  const err = { message: 'Animal does not exist' };
  return res.status(400).json(err);
});

// edit a visit
router.put('/:id', validate(Visit.validation.update), async (req, res) => {
  const { id } = req.params;
  const { animalId } = req.body;
  const visit = await Visit.update(req.params.id, req.body);

  if (visit) {
    if (animalId) {
      await Visit.updateAnimal(id, animalId);
      await Animal.removeVisit(animalId, id);
    }

    return res.status(200).json(visit);
  }

  return res.sendStatus(404);
});

// delete a visit
router.delete('/:id', async (req, res) => {
  const visitId = req.params.id;
  const visit = await Visit.readOne(visitId);

  if (visit) {
    // remove the visit from the animals visits
    await Animal.removeVisit(visit.attributes.animalId, visitId);
  }

  await Visit.delete(visitId);
  res.sendStatus(204);
});

module.exports = router;
