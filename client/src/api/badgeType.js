import options from './options';


export const getBadgeTypes = () => fetch('/badgeType').then(res => res.json());
export const createBadgeType = (values) => fetch('/badgeType', { method: 'post', headers: options.headers, body: JSON.stringify(values) });
export const createRandomBadgeType = () => fetch('/badgeType/random', { method: 'post' });
export const deleteBadgeType = (id) => fetch(`/badgeType/${id}`, { method: 'delete' });
export const editBadgeType = ({ id, ...values }) => fetch(`/badgeType/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) });
