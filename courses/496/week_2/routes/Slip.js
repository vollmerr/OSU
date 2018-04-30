const Joi = require('joi');
const router = require('express').Router();

const Boat = require('../store/Boat');
const Slip = require('../store/Slip');

// gets all slips
router.get('/', async (req, res) => {
    const [slips] = await Slip.get();
    res.json(slips);
});

// get a slip by id
router.get('/:id', async (req, res) => {
    const [slip] = await Slip.getById(req.params.id);
    res.json(slip);
});

// get a slip's boat by id
router.get('/:id/boat', async (req, res) => {
    const [slip] = await Slip.getById(req.params.id);
    const [boat] = await Boat.getById(slip[Slip.schema.currentBoat]);
    res.json(boat);
});

// edit a slip by id
router.put('/:id', async (req, res) => {
    const schema = {
        [Slip.schema.number]: Joi.number().integer(),
        [Slip.schema.arrivalDate]: Joi.date(),
        [Slip.schema.currentBoat]: Joi.string(),
    };
    const { error, value } = Joi.validate(req.body, schema);
    if (error) { return res.status(400).json(error); }
    // edit slip by id
    const id = req.params.id;
    // edit the slip
    const editedSlip = await Slip.update({ ...value, id });
    // if null, means already assigned
    if (!editedSlip) {
        return res.sendStatus(403);
    }
    // if a boat was assigned, update it to be at sea
    const boatId = editedSlip[Slip.schema.currentBoat];
    if (boatId) {
        await Boat.update({ [Boat.schema.id]: boatId, [Boat.schema.atSea]: false });
    }
    res.json(editedSlip);
});

// delete a slip by id
router.delete('/:id', async (req, res) => {
    const id = req.params.id;
    // get the slip
    const [slip] = await Slip.getById(id);
    // boat is assigned
    if (slip && slip[Slip.schema.currentBoat]) {
        // send it to sea
        await Boat.update({ id: slip[Slip.schema.currentBoat], [Boat.schema.atSea]: true });
    }
    await Slip.del(id);
    res.sendStatus(204);
});

// create a new slip
router.post('/', async (req, res) => {
    const schema = {
        [Slip.schema.number]: Joi.number().integer().required(),
    };
    // validate for schema errors
    const { error, value } = Joi.validate(req.body, schema);
    if (error) { return res.status(400).json(error); }
    // make a new slip...
    const newSlip = await Slip.create(value);
    res.json(newSlip);
});


module.exports = router;