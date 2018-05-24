const Joi = require('joi');
const uuid = require('uuid/v4');
const nconf = require('nconf');

const date = require('../utils/date');
const ds = require('./connect');

const kind = 'Visit';
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
      self: `${nconf.get('APP_URL')}/visits/${id}`,
    },
  };

  await ds.insert({ data, key: ds.key([kind, id]) });
  return data;
};

exports.read = async () => {
  const query = ds.createQuery(kind);
  const [visits] = await ds.runQuery(query);
  return visits;
};

exports.readOne = async (id) => {
  const key = ds.key([kind, id]);
  const [visit] = await ds.get(key);
  return visit;
};

exports.update = async (id, values) => {
  const visit = await exports.read(id);
  const key = ds.key([kind, id]);
  const data = {
    ...visit,
    ...values,
    updatedAt: date.now(),
  };

  await ds.update({ key, data });
  return data;
};

exports.delete = async (id) => {
  const key = ds.key([kind, id]);
  return ds.delete(key);
};
