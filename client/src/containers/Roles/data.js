import { mapToColumns } from '../../utils/data';

import { ROLE } from './constants';


export const role = {
  [ROLE.ID]: {
    label: 'ID',
    name: ROLE.ID,
    ariaLabel: 'ID of the role record',
    minWidth: 20,
    maxWidth: 40,
  },
  [ROLE.NAME]: {
    label: 'Role Name',
    name: ROLE.NAME,
    ariaLabel: 'Name of the role',
    minWidth: 150,
  },
};


export const columns = mapToColumns(role);
