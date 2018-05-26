const router = require('express').Router();

const validate = require('../middleware/validate');
const Visit = require('../store/Visit');
const Animal = require('../store/Animal');

// view all visits
router.get('/', async (req, res) => {
  const visits = await Visit.read(req.user.id);

  if (visits) {
    return res.status(200).json(visits);
  }

  return res.sendStatus(404);
});

// view a visit by id
router.get('/:id', async (req, res) => {
  const visit = await Visit.readOne(req.user.id, req.params.id);

  if (visit) {
    return res.status(200).json(visit);
  }

  return res.sendStatus(404);
});

// view a visits animal
router.get('/:id/animal', async (req, res) => {
  const visit = await Visit.readOne(req.user.id, req.params.id);

  if (visit) {
    const animal = await Animal.readOne(req.user.id, visit.id);

    if (animal) {
      return res.status(200).json(animal);
    }
  }

  return res.sendStatus(404);
});

// create a new visit
router.post('/', validate(Visit.validation.create), async (req, res) => {
  const animal = await Animal.readOne(req.user.id, req.body.animalId);

  // add visit to the animals visits
  if (animal) {
    const visit = await Visit.create(req.user.id, req.body);
    if (visit) {
      const data = await Animal.addVisit(req.user.id, animal.id, visit.id);
      if (data) {
        return res.status(201).json(visit);
      }
    }
  }

  const err = { message: 'Invalid animal' };
  return res.status(400).json(err);
});

// edit a visit
router.put('/:id', validate(Visit.validation.update), async (req, res) => {
  const { id } = req.params;
  const { animalId } = req.body;
  const visit = await Visit.update(req.user.id, req.params.id, req.body);

  if (visit) {
    if (animalId) {
      await Visit.updateAnimal(req.user.id, id, animalId);
      await Animal.removeVisit(req.user.id, animalId, id);
    }

    return res.status(200).json(visit);
  }

  return res.sendStatus(404);
});

// delete a visit
router.delete('/:id', async (req, res) => {
  const visitId = req.params.id;
  const visit = await Visit.readOne(req.user.id, visitId);

  if (visit) {
    // remove the visit from the animals visits
    await Animal.removeVisit(req.user.id, visit.attributes.animalId, visitId);
    await Visit.delete(req.user.id, visitId);
  }

  res.sendStatus(204);
});

module.exports = router;
