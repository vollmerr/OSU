import Equipment from '../Equipment';
import Campus from '../Campus';
import Locations from '../Locations';
import BadgeTypes from '../BadgeTypes';
import Roles from '../Roles';
import Admins from '../Admins';


const routes = {
  equipment: {
    path: '/equipment',
    text: 'Equipment',
    key: 'equipment',
    exact: true,
    component: Equipment,
  },
  campus: {
    path: '/campus',
    text: 'Campus',
    key: 'campus',
    exact: true,
    component: Campus,
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
  roles: {
    path: '/roles',
    text: 'Roles',
    key: 'roles',
    exact: true,
    component: Roles,
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
