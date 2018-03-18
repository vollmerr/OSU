const options = {
  headers: {
    'user-agent': 'Mozilla/4.0 MDN Example',
    'content-type': 'application/json'
  },
};

const buildRoutes = ({ base, ...rest }) => ({
  get: () => fetch(`/${base}`).then(res => res.json()),
  create: (values) => fetch(`/${base}`, { method: 'post', headers: options.headers, body: JSON.stringify(values) }),
  createRandom: () => fetch(`/${base}/random`, { method: 'post' }),
  delete: (id) => fetch(`/${base}/${id}`, { method: 'delete' }),
  edit: ({ id, ...values }) => fetch(`/${base}/${id}`, { method: 'put', headers: options.headers, body: JSON.stringify(values) }),
  ...rest,
});


export default {
  equipment: buildRoutes({ base: 'equipment' }),
  campus: buildRoutes({ base: 'campus' }),
  location: buildRoutes({ base: 'location' }),
  campusLocation: buildRoutes({ base: 'campusLocation' }),
  badgeType: buildRoutes({ base: 'badgeType' }),
  role: buildRoutes({ base: 'role' }),
  admin: buildRoutes({ base: 'admin' }),
  visitor: buildRoutes({ base: 'visitor' }),
}
