/* eslint-disable no-param-reassign */
const Joi = require('joi');
const uuid = require('uuid/v4');
const nconf = require('nconf');

const date = require('../utils/date');
const ds = require('./connect');

const kind = 'visit';
const schema = {
  id: Joi.string(),
  startDate: Joi.date(),
  endDate: Joi.date(),
  reason: Joi.string(),
  animalId: Joi.string(),
  createdAt: Joi.date(),
  updatedAt: Joi.date(),
};

exports.kind = kind;
exports.schema = schema;
exports.validation = {
  create: Joi.object({
    startDate: schema.startDate.required(),
    endDate: schema.endDate.required(),
    reason: schema.reason.required(),
    animalId: schema.animalId.required(),
  }),
  update: Joi.object({
    startDate: schema.startDate,
    endDate: schema.endDate,
    reason: schema.reason,
    animalId: schema.animalId,
  }),
};

// create new visit
exports.create = async (ownerId, values) => {
  const id = uuid();
  const link = `${nconf.get('APP_URL')}/visits/${id}`;
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
      animal: {
        links: {
          self: `${link}/animal`,
        },
        data: { type: 'animal', id: values.animalId },
      },
    },
  };

  await ds.insert({ data, key: ds.key([kind, id]) });

  return data;
};

// get all visits
exports.read = async (ownerId) => {
  const query = ds.createQuery(kind).filter('attributes.ownerId', '=', ownerId);
  const [visits] = await ds.runQuery(query);

  return visits;
};

// get a visit by id
exports.readOne = async (ownerId, id) => {
  const key = ds.key([kind, id]);
  const [visit] = await ds.get(key);

  if (!visit || visit.attributes.ownerId !== ownerId) {
    return null;
  }

  return visit;
};

// get a visit by animalId
exports.findByAnimalId = async (ownerId, animalId) => {
  const query = ds
    .createQuery(kind)
    .filter('attributes.animalId', '=', animalId)
    .filter('attributes.ownerId', '=', ownerId);
  const [visits] = await ds.runQuery(query);

  return visits;
};

// helper function for retrieving the visit and applying updates to it
exports.applyUpdates = async (ownerId, id, mutator) => {
  const visit = await exports.readOne(ownerId, id);
  const key = ds.key([kind, id]);

  if (!visit) {
    return null;
  }

  // mutate animal based off function passed in to apply updates
  mutator(visit);
  visit.attributes.updatedAt = date.now();

  await ds.update({ key, data: visit });
  return visit;
};

// update attributes
exports.update = async (ownerId, id, values) => {
  const mutator = (data) => {
    data.attributes = { ...data.attributes, ...values };
  };

  return exports.applyUpdates(ownerId, id, mutator);
};

// update animal relationship
exports.updateAnimal = async (ownerId, id, animalId) => {
  const mutator = (data) => {
    data.relationships.animal.data.id = animalId;
  };

  return exports.applyUpdates(ownerId, id, mutator);
};

// delete all visits that have animalId
exports.deleteByAnimalId = async (ownerId, animalId) => {
  const visits = await exports.findByAnimalId(ownerId, animalId);

  if (!visits) {
    return null;
  }

  const keys = visits.map((x) => ds.key([kind, x.id]));
  return ds.delete(keys);
};

// delete visit by id
exports.delete = async (ownerId, id) => {
  const visit = exports.readOne(ownerId, id);

  if (!visit) {
    return null;
  }

  const key = ds.key([kind, id]);
  return ds.delete(key);
};
