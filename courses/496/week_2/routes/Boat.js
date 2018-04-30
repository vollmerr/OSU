const Joi = require('joi');
const router = require('express').Router();


const Boat = require('../store/Boat');
const Slip = require('../store/Slip');


// gets all boats
router.get('/', async (req, res) => {
    const [boats] = await Boat.get();
    res.json(boats);
});

// get a boat by id
router.get('/:id', async (req, res) => {
    const [boat] = await Boat.getById(req.params.id);
    res.json(boat);
});

// edit a boat by id
router.put('/:id', async (req, res) => {
    const schema = {
        [Boat.schema.name]: Joi.string(),
        [Boat.schema.type]: Joi.string(),
        [Boat.schema.length]: Joi.number().integer(),
        [Boat.schema.atSea]: Joi.boolean(),
    };
    const { error, value } = Joi.validate(req.body, schema);
    if (error) { return res.status(400).json(error); }
    // edit boat by id
    const id = req.params.id;
    // if set to sea
    if (value[Boat.schema.atSea]) {
        // set slip to being empty if in one
        await Slip.empty(id);
    }
    const editedBoat = await Boat.update({ ...value, id });
    res.json(editedBoat);
});

// delete a boat by id
router.delete('/:id', async (req, res) => {
    const id = req.params.id;
    await Slip.empty(id);
    await Boat.del(id);
    res.sendStatus(204);
});

// create a new boat
router.post('/', async (req, res) => {
    const schema = {
        [Boat.schema.name]: Joi.string().required(),
        [Boat.schema.type]: Joi.string().required(),
        [Boat.schema.length]: Joi.number().integer().required(),
    };
    // validate for schema errors
    const { error, value } = Joi.validate(req.body, schema);
    if (error) { return res.status(400).json(error); }
    // make the new boat...
    const newBoat = await Boat.create(value);
    res.json(newBoat);
});


module.exports = router;