const router = require('express').Router();

const Animal = require('../store/Animal');

router.get('/', Animal.read);
router.get('/:id', Animal.read);
router.post('/', Animal.create);
router.put('/:id', Animal.update);
router.delete('/:id', Animal.delete);

module.exports = router;
