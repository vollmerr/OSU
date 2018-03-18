import options from './options';


export const getRoles = () => fetch('/role').then(res => res.json());
export const createRole = (values) => fetch('/role', { method: 'post', headers: options.headers, body: JSON.stringify(values) });
export const createRandomRole = () => fetch('/role/random', { method: 'post' });
export const deleteRole = (id) => fetch(`/role/${id}`, { method: 'delete' });
export const editRole = ({ id, ...values }) => fetch(`/role/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) });
