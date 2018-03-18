import options from './options';


export const getLocations = () => fetch('/location').then(res => res.json());
export const createLocation = (values) => fetch('/location', { method: 'post', headers: options.headers, body: JSON.stringify(values) });
export const createRandomLocation = () => fetch('/location/random', { method: 'post' });
export const deleteLocation = (id) => fetch(`/location/${id}`, { method: 'delete' });
export const editLocation = ({ id, ...values }) => fetch(`/location/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) });
