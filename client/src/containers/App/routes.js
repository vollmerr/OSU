import Equipment from '../Equipment';


const routes = {
  equipment: {
    path: '/equipment',
    text: 'Equipment',
    key: 'equipment',
    exact: true,
    component: Equipment,
  },
};


export const home = routes.equipment;
export default Object.values(routes);