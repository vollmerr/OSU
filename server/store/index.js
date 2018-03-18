const con = require('./connect');

const C = require('./constants');


// calls database - handles opening and closing connection
const query = (query) => (
  new Promise((resolve, reject) => {
    return con.query(query, (err, result) => {
      if (err) {
        return reject(err);
      }
      return resolve(result);
    });
  })
);


// EQUIPMENT
const equipment = {
  // create table
  create: () => (
    query(`create table equipment(
        ${C.EQUIPMENT.ID} int primary key auto_increment,
        ${C.EQUIPMENT.NAME} varchar(255) not null
    )`)
  ),
  // drop table
  drop: () => (
    query('drop table if exists equipment')
  ),
  // get all
  get: () => (
    query('select * from equipment')
  ),
  // create a equipment
  insert: (values) => (
    query(`insert into equipment (${Object.keys(values).join(', ')}) values ("${Object.values(values).join('", "')}")`)
  ),
  // deletes a equipment by id
  delete: (id) => (
    query(`delete from equipment where id=${id}`)
  ),
  // edits a equipment by id
  edit: ({ id, ...rest }) => (
    query(`update equipment set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where ${C.EQUIPMENT.ID}=${id}`)
  ),
};

const buildStore = ({ name, structure }) => ({
  // create table
  create: () => (
    query(structure)
  ),
  // drop table
  drop: () => (
    query(`drop table if exists ${name}`)
  ),
  // get all
  get: (columns = '*') => (
    query(`select ${columns} from ${name}`)
  ),
  // create a equipment
  insert: (values) => (
    query(`insert into ${name} (${Object.keys(values).join(', ')}) values ("${Object.values(values).join('", "')}")`)
  ),
  // deletes a equipment by id
  delete: (id) => (
    query(`delete from ${name} where id=${id}`)
  ),
  // edits a equipment by id
  edit: ({ id, ...rest }) => (
    query(`update ${name} set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where id=${id}`)
  ),
});

// // ROLE
// const role = {
//   // create table
//   create: () => (
//     query(`create table role(
//       ${C.ROLE.ID} int primary key auto_increment,
//       ${C.ROLE.NAME} varchar(255) not null
//     )`)
//   ),
//   // drop table
//   drop: () => (
//     query('drop table if exists role')
//   ),
//   // get all
//   get: () => (
//     query('select * from role')
//   ),
//   // create a equipment
//   insert: (values) => (
//     query(`insert into role (${C.ROLE.NAME}) values ("${values[C.ROLE.NAME]}")`)
//   ),
//   // deletes a equipment by id
//   delete: (id) => (
//     query(`delete from role where id=${id}`)
//   ),
//   // edits a equipment by id
//   edit: ({ id, ...rest }) => (
//     query(`update role set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where id=${id}`)
//   ),
// };

const role = buildStore({
  name: 'role',
  structure: `
    create table role(
      ${C.ROLE.ID} int primary key auto_increment,
      ${C.ROLE.NAME} varchar(255) not null
    )
  `,
});


// ADMIN
const admin = {
  // create table
  create: () => (
    query(`create table admin(
        ${C.ADMIN.ID} int primary key auto_increment,
        ${C.ADMIN.FIRST_NAME} varchar(255) not null,
        ${C.ADMIN.LAST_NAME} varchar(255) not null,
        ${C.ADMIN.ROLE_ID} int not null,
        foreign key fk_roleId(${C.ADMIN.ROLE_ID}) references role(${C.ROLE.ID})
    )`)
  ),
  // drop table
  drop: () => (
    query('drop table if exists admin')
  ),
  // get all
  get: () => (
    query('select * from admin')
  ),
  // create a admin
  insert: (values) => (
    query(`insert into admin (${C.ROLE.NAME}) values ("${values[C.ROLE.NAME]}")`)
  ),
  // deletes a admin by id
  delete: (id) => (
    query(`delete from admin where id=${id}`)
  ),
  // edits a admin by id
  edit: ({ id, ...rest }) => (
    query(`update admin set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where id=${id}`)
  ),
};


module.exports = {
  con,
  equipment,
  role,
  admin,
};
