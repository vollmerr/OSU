export const getUsers = () => {
    return fetch('/users').then(res => res.json())
};

export const newRandomUser = () => {
    return fetch('/users/create', { method: 'post' });
};

export const deteteUser = (id) => {
    return fetch(`/users/${id}/delete`, { method: 'post' });
};