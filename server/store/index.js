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


const buildStore = ({ name, structure }) => ({
  // create table
  create: () => (
    query(`create table ${name}(${structure})`)
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


const equipment = buildStore({
  name: 'equipment',
  structure: `
    ${C.EQUIPMENT.ID} int primary key auto_increment,
    ${C.EQUIPMENT.NAME} varchar(255) not null
  `,
});


const role = buildStore({
  name: 'role',
  structure: `
    ${C.ROLE.ID} int primary key auto_increment,
    ${C.ROLE.NAME} varchar(255) not null
  `,
});

const admin = buildStore({
  name: 'admin',
  structure: `
    ${C.ADMIN.ID} int primary key auto_increment,
    ${C.ADMIN.FIRST_NAME} varchar(255) not null,
    ${C.ADMIN.LAST_NAME} varchar(255) not null,
    ${C.ADMIN.ROLE_ID} int not null,
    foreign key fk_roleId(${C.ADMIN.ROLE_ID}) references role(${C.ROLE.ID})
  `,
});


module.exports = {
  con,
  equipment,
  role,
  admin,
};
