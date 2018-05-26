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
  visits: Joi.array(),
  ownerFirstName: Joi.string(),
  ownerLastName: Joi.string(),
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
    ownerFirstName: schema.ownerFirstName.required(),
    ownerLastName: schema.ownerLastName.required(),
  }),
  update: Joi.object({
    name: schema.name,
    age: schema.age,
    type: schema.type,
    ownerFirstName: schema.ownerFirstName,
    ownerLastName: schema.ownerLastName,
  }),
};

// create new animal
exports.create = async (values) => {
  const id = uuid();
  const link = `${nconf.get('APP_URL')}/animals/${id}`;
  const data = {
    type: kind,
    id,
    attributes: {
      ...values,
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
exports.read = async () => {
  const query = ds.createQuery(kind);
  const [animals] = await ds.runQuery(query);
  return animals;
};

// get one animal by id
exports.readOne = async (id) => {
  const key = ds.key([kind, id]);
  const [animal] = await ds.get(key);
  return animal;
};

// helper function for retrieving the animal and applying updates to it
exports.applyUpdates = async (id, mutator) => {
  const animal = await exports.readOne(id);
  const key = ds.key([kind, id]);

  // mutate animal based off function passed in to apply updates
  mutator(animal);
  animal.attributes.updatedAt = date.now();

  await ds.update({ key, data: animal });
  return animal;
};

// update attributes
exports.update = async (id, values) => {
  const mutator = (data) => {
    data.attributes = { ...data.attributes, ...values };
  };

  return exports.applyUpdates(id, mutator);
};

// add a visit to its relationships
exports.addVisit = async (id, visitId) => {
  const mutator = (data) => {
    data.relationships.visits.data.push({ type: 'Visit', id: visitId });
  };

  return exports.applyUpdates(id, mutator);
};

// remove a visit from its relationships
exports.removeVisit = async (id, visitId) => {
  const mutator = (data) => {
    data.relationships.visits.data = data.relationships.visits.data.filter(
      (x) => x.id !== visitId
    );
  };

  return exports.applyUpdates(id, mutator);
};

// delete a animal by id
exports.delete = async (id) => {
  const key = ds.key([kind, id]);
  return ds.delete(key);
};
