/* eslint-disable no-param-reassign */
const Joi = require('joi');
const uuid = require('uuid/v4');
const nconf = require('nconf');

const date = require('../utils/date');
const ds = require('./connect');

const kind = 'animal';
const schema = {
  id: Joi.string(),
  name: Joi.string(),
  age: Joi.string(),
  type: Joi.string(),
  weight: Joi.string(),
  visits: Joi.array(),
  ownerId: Joi.string(),
  createdAt: Joi.date(),
  updatedAt: Joi.date(),
};

exports.kind = kind;
exports.schema = schema;
exports.validation = {
  create: Joi.object({
    name: schema.name.required(),
    age: schema.age.required(),
    type: schema.type.required(),
    weight: schema.weight.required(),
  }),
  update: Joi.object({
    name: schema.name,
    age: schema.age,
    type: schema.type,
    weight: schema.weight,
  }),
};

// create new animal
exports.create = async (ownerId, values) => {
  const id = uuid();
  const link = `${nconf.get('APP_URL')}/animals/${id}`;
  const data = {
    type: kind,
    id,
    attributes: {
      ...values,
      ownerId,
      createdAt: date.now(),
      updatedAt: date.now(),
    },
    links: {
      self: link,
    },
    relationships: {
      visits: {
        links: {
          self: `${link}/visits`,
        },
        data: [],
      },
    },
  };

  await ds.insert({ data, key: ds.key([kind, id]) });
  return data;
};

// get all animals
exports.read = async (ownerId) => {
  const query = ds.createQuery(kind).filter('attributes.ownerId', '=', ownerId);
  const [animals] = await ds.runQuery(query);

  return animals;
};

// get one animal by id
exports.readOne = async (ownerId, id) => {
  const key = ds.key([kind, id]);
  const [animal] = await ds.get(key);

  if (!animal || animal.attributes.ownerId !== ownerId) {
    return null;
  }

  return animal;
};

// helper function for retrieving the animal and applying updates to it
exports.applyUpdates = async (ownerId, id, mutator) => {
  const animal = await exports.readOne(ownerId, id);
  const key = ds.key([kind, id]);

  if (!animal) {
    return null;
  }

  // mutate animal based off function passed in to apply updates
  mutator(animal);
  animal.attributes.updatedAt = date.now();

  await ds.update({ key, data: animal });
  return animal;
};

// update attributes
exports.update = async (ownerId, id, values) => {
  const mutator = (data) => {
    data.attributes = { ...data.attributes, ...values };
  };

  return exports.applyUpdates(ownerId, id, mutator);
};

// add a visit to its relationships
exports.addVisit = async (ownerId, id, visitId) => {
  const mutator = (data) => {
    data.relationships.visits.data.push({ type: 'Visit', id: visitId });
  };

  return exports.applyUpdates(ownerId, id, mutator);
};

// remove a visit from its relationships
exports.removeVisit = async (ownerId, id, visitId) => {
  const mutator = (data) => {
    data.relationships.visits.data = data.relationships.visits.data.filter(
      (x) => x.id !== visitId
    );
  };

  return exports.applyUpdates(ownerId, id, mutator);
};

// delete a animal by id
exports.delete = async (ownerId, id) => {
  const animal = exports.readOne(ownerId, id);

  if (!animal || animal.id !== ownerId) {
    return null;
  }

  const key = ds.key([kind, id]);
  return ds.delete(key);
};
