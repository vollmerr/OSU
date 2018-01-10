import HomePage from 'containers/HomePage';
import CS340 from 'cs_340';


const routes = {
    home: {
        path: '/',
        text: 'Home',
        key: 'home',
        exact: true,
        component: HomePage,
    },
    cs340: {
        path: '/cs340',
        text: 'CS 340',
        key: 'cs340',
        component: CS340,
    }
};

export const home = routes.home;
export default Object.values(routes);