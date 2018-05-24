const uuid = require('uuid/v4');

const ds = require('./connect');

exports.kind = 'Animal';
exports.schema = {
  id: 'id',
  name: 'name',
  age: 'age',
  type: 'type',
  ownerId: 'ownerId',
  visits: 'visits',
  createdAt: 'createdAt',
  updatedAt: 'updatedAt',
};

exports.create = async (req, res) => {
  res.status(201);
};

exports.read = async (req, res) => {
  res.status(200);
};

exports.update = async (req, res) => {
  res.status(200);
};

exports.delete = async (req, res) => {
  res.status(204);
};
