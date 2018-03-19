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

// base for building a generic store
const buildStore = ({ name, structure, ...rest }) => ({
  // create table
  create: () => (
    query(`create table ${name}(${structure})`)
  ),
  // drop table
  drop: () => (
    query(`drop table if exists ${name}`)
  ),
  // get all
  get: ({ columns = '*' }) => (
    query(`select ${columns} from ${name}`)
  ),
  // create a single row
  insert: (values) => (
    query(`insert into ${name} (${Object.keys(values).join(', ')}) values ("${Object.values(values).join('", "')}")`)
  ),
  // deletes a row by id
  delete: ({ id }) => (
    query(`delete from ${name} where id=${id}`)
  ),
  // edit a row by id
  edit: ({ id, ...rest }) => (
    query(`update ${name} set ${Object.keys(rest).map(x => `${x} = '${rest[x]}'`).join(', ')} where id=${id}`)
  ),
  // extend for custom overrides (ex custom 'get')
  ...rest,
});


const role = buildStore({
  name: 'role',
  structure: `
    ${C.ROLE.ID} int primary key auto_increment,
    ${C.ROLE.NAME} varchar(255) not null
  `,
});

const campus = buildStore({
  name: 'campus',
  structure: `
    ${C.CAMPUS.ID} int primary key auto_increment,
    ${C.CAMPUS.NAME} varchar(255) not null
  `,
});


const location = buildStore({
  name: 'location',
  structure: `
    ${C.LOCATION.ID} int primary key auto_increment,
    ${C.LOCATION.NAME} varchar(255) not null
  `,
});

const campusLocation = buildStore({
  name: 'campusLocation',
  structure: `
    ${C.CAMPUS_LOCATION.ID} int primary key auto_increment,
    ${C.CAMPUS_LOCATION.CAMPUS_ID} int not null,  
    ${C.CAMPUS_LOCATION.LOCATION_ID} int not null,
    foreign key fk_campusId(${C.CAMPUS_LOCATION.CAMPUS_ID}) references campus(${C.CAMPUS.ID}),
    foreign key fk_locationId(${C.CAMPUS_LOCATION.LOCATION_ID}) references location(${C.LOCATION.ID}),
    constraint c_campusLocation unique (${C.CAMPUS_LOCATION.CAMPUS_ID}, ${C.CAMPUS_LOCATION.LOCATION_ID})
  `,
  get: ({ where }) => (
    query(`
      select cl.${C.CAMPUS_LOCATION.ID}, cl.${C.CAMPUS_LOCATION.CAMPUS_ID}, cl.${C.CAMPUS_LOCATION.LOCATION_ID}, 
        c.${C.CAMPUS.NAME} as campusName, 
        l.${C.LOCATION.NAME} as locationName 
      from campusLocation cl 
      inner join campus c on c.${C.CAMPUS.ID}=cL.${C.CAMPUS_LOCATION.CAMPUS_ID}
      inner join location l on l.${C.LOCATION.ID}=cL.${C.CAMPUS_LOCATION.LOCATION_ID}
    `)
  ),
});

const badgeType = buildStore({
  name: 'badgeType',
  structure: `
    ${C.BADGE_TYPE.ID} int primary key auto_increment,
    ${C.BADGE_TYPE.TYPE} varchar(255) not null
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
  get: () => (
    query(`
      select a.${C.ADMIN.ID}, a.${C.ADMIN.FIRST_NAME}, a.${C.ADMIN.LAST_NAME}, 
        r.${C.ROLE.NAME} as roleName, r.${C.ROLE.ID} as roleId
      from admin a
      inner join role r on r.${C.ROLE.ID}=a.${C.ADMIN.ROLE_ID}
    `)
  ),
});

const visit = buildStore({
  name: 'visit',
  structure: `
    ${C.VISIT.ID} int primary key auto_increment,
    ${C.VISIT.DATE} datetime not null,
    ${C.VISIT.CAMPUS_LOCATION_ID} int not null,
    foreign key fk_campusLocationId(${C.VISIT.CAMPUS_LOCATION_ID}) references campusLocation(${C.CAMPUS_LOCATION.ID})
  `,
  get: () => (
    query(`
      select v.${C.VISIT.ID}, v.${C.VISIT.DATE}, 
        cl.${C.CAMPUS_LOCATION.ID} as campusLocationId, cl.${C.CAMPUS_LOCATION.CAMPUS_ID}, cl.${C.CAMPUS_LOCATION.LOCATION_ID},
        c.${C.CAMPUS.NAME} as campusName,
        l.${C.LOCATION.NAME} as locationName
      from visit v 
      inner join campusLocation cl on cl.${C.CAMPUS_LOCATION.ID}=v.${C.VISIT.CAMPUS_LOCATION_ID}
      inner join campus c on c.${C.CAMPUS.ID}=cl.${C.CAMPUS_LOCATION.CAMPUS_ID}
      inner join location l on l.${C.LOCATION.ID}=cl.${C.CAMPUS_LOCATION.LOCATION_ID}
    `)
  ),
});

const visitor = buildStore({
  name: 'visitor',
  structure: `
    ${C.VISITOR.ID} int primary key auto_increment,
    ${C.VISITOR.FIRST_NAME} varchar(255) not null,
    ${C.VISITOR.LAST_NAME} varchar(255) not null,
    ${C.VISITOR.BADGE_ID} int not null,
    ${C.VISITOR.VISIT_ID} int not null,
    foreign key fk_badgeId(${C.VISITOR.BADGE_ID}) references badgeType(${C.BADGE_TYPE.ID}),
    foreign key fk_visitId(${C.VISITOR.VISIT_ID}) references visit(${C.VISIT.ID})
  `,
  get: () => (
    query(`
      select v.${C.VISITOR.ID}, ${C.VISITOR.FIRST_NAME}, ${C.VISITOR.LAST_NAME}, 
        b.${C.BADGE_TYPE.ID} as badgeId, b.${C.BADGE_TYPE.TYPE} as badgeType, 
        t.${C.VISIT.ID} as visitId, t.${C.VISIT.DATE} as visitDate,
        cl.${C.CAMPUS_LOCATION.ID} as campusLocationId,
        c.${C.CAMPUS.ID} as campusId, c.${C.CAMPUS.NAME} as campusName,
        l.${C.LOCATION.ID} as locationId, l.${C.LOCATION.NAME} as locationName
      from visitor v 
      inner join badgeType b on b.${C.BADGE_TYPE.ID}=v.${C.VISITOR.BADGE_ID}
      inner join visit t on t.${C.VISIT.ID}=v.${C.VISITOR.VISIT_ID}
      inner join campusLocation cl on cl.${C.CAMPUS_LOCATION.ID}=t.${C.VISIT.CAMPUS_LOCATION_ID}
      inner join campus c on c.${C.CAMPUS.ID}=cl.${C.CAMPUS_LOCATION.CAMPUS_ID} 
      inner join location l on l.${C.LOCATION.ID}=cl.${C.CAMPUS_LOCATION.LOCATION_ID}
    `)
  ),
});

// order of exports matter for migrate / rollback!
module.exports = {
  con,
  role,
  campus,
  location,
  campusLocation,
  badgeType,
  admin,
  visit,
  visitor,
};
