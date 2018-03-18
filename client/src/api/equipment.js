import options from './options';


export const getEquipment = () => fetch('/equipment').then(res => res.json());
export const createEquipment = (values) => fetch('/equipment', { method: 'post', headers: options.headers, body: JSON.stringify(values) });
export const createRandomEquipment = () => fetch('/equipment/random', { method: 'post' });
export const deleteEquipment = (id) => fetch(`/equipment/${id}`, { method: 'delete' });
export const editEquipment = ({ id, ...values }) => fetch(`/equipment/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) });