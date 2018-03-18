import Equipment from '../Equipment';
import Admins from '../Admins';
import Roles from '../Roles';
import Locations from '../Locations';
import BadgeTypes from '../BadgeTypes';


const routes = {
  equipment: {
    path: '/equipment',
    text: 'Equipment',
    key: 'equipment',
    exact: true,
    component: Equipment,
  },
  roles: {
    path: '/roles',
    text: 'Roles',
    key: 'roles',
    exact: true,
    component: Roles,
  },
  location: {
    path: '/locations',
    text: 'Locations',
    key: 'locations',
    exact: true,
    component: Locations,
  },
  badgeTypes: {
    path: '/badgeTypes',
    text: 'Badge Types',
    key: 'badgeTypes',
    exact: true,
    component: BadgeTypes,
  },
  admin: {
    path: '/admins',
    text: 'Admins',
    key: 'admins',
    exact: true,
    component: Admins,
  },
};


export const home = routes.equipment;
export default Object.values(routes);
