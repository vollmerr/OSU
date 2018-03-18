const con = require('./connect');


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
  createTableEquipment: () => (
    query(`create table equipment(
        id int primary key auto_increment,
        name varchar(255) not null
    )`)
  ),
  // drop table
  dropTableEquipment: () => (
    query('drop table if exists equipment')
  ),
  // get all
  getEquipment: () => (
    query('select * from equipment')
  ),
  // create a equipment
  createEquipment: ({ name }) => (
    query(`insert into equipment (name) values ("${name}")`)
  ),
  // deletes a equipment by id
  deleteEquipment: (id) => (
    query(`delete from equipment where id=${id}`)
  ),
  // edits a equipment by id
  editEquipment: ({ id, ...rest }) => (
    query(`update equipment set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where id=${id}`)
  ),
};


module.exports = {
  con,
  equipment,
};
