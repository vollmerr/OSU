import Equipment from '../Equipment';
import Campus from '../Campus';
import Locations from '../Locations';
import CampusLocations from '../CampusLocations';
import BadgeTypes from '../BadgeTypes';
import Roles from '../Roles';
import Admins from '../Admins';
import Visitors from '../Visitors';


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
  locations: {
    path: '/locations',
    text: 'Locations',
    key: 'locations',
    exact: true,
    component: Locations,
  },
  campusLocations: {
    path: '/campusLocations',
    text: 'Campus-Locations',
    key: 'campusLocations',
    exact: true,
    component: CampusLocations,
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
  admins: {
    path: '/admins',
    text: 'Admins',
    key: 'admins',
    exact: true,
    component: Admins,
  },
  visitors: {
    path: '/visitors',
    text: 'Visitors',
    key: 'visitors',
    exact: true,
    component: Visitors,
  },
};


export const home = routes.equipment;
export default Object.values(routes);
