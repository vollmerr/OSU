import options from './options';


export const getAdmins = () => fetch('/admin').then(res => res.json());
export const createAdmin = (values) => fetch('/admin', { method: 'post', headers: options.headers, body: JSON.stringify(values) });
export const createRandomAdmin = () => fetch('/admin/random', { method: 'post' });
export const deleteAdmin = (id) => fetch(`/admin/${id}`, { method: 'delete' });
export const editAdmin = ({ id, ...values }) => fetch(`/admin/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) });
