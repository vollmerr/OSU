const Joi = require('joi');
const uuid = require('uuid/v4');
const nconf = require('nconf');

const date = require('../utils/date');
const ds = require('./connect');

const kind = 'Animal';
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

exports.create = async (values) => {
  const id = uuid();
  const data = {
    type: kind,
    id,
    attributes: {
      ...values,
      createdAt: date.now(),
      updatedAt: date.now(),
    },
    links: {
      self: `${nconf.get('APP_URL')}/animals/${id}`,
    },
  };

  await ds.insert({ data, key: ds.key([kind, id]) });
  return data;
};

exports.read = async () => {
  const query = ds.createQuery(kind);
  const [animals] = await ds.runQuery(query);
  return animals;
};

exports.readOne = async (id) => {
  const key = ds.key([kind, id]);
  const [animal] = await ds.get(key);
  return animal;
};

exports.update = async (id, values) => {
  const animal = await exports.readOne(id);
  const key = ds.key([kind, id]);
  const data = {
    ...animal,
    attributes: {
      ...animal.attributes,
      ...values,
      updatedAt: date.now(),
    },
  };
  await ds.update({ key, data });
  return data;
};

exports.delete = async (id) => {
  const key = ds.key([kind, id]);
  return ds.delete(key);
};
