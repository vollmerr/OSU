import Equipment from '../Equipment';
import Admins from '../Admins';
import Roles from '../Roles';


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
