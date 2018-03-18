import { mapToColumns } from '../../utils/data';

import { ADMIN } from './constants';


export const admin = {
  [ADMIN.ID]: {
    label: 'ID',
    name: ADMIN.ID,
    ariaLabel: 'ID of the admin record',
    minWidth: 20,
    maxWidth: 40,
  },
  [ADMIN.FIRST_NAME]: {
    label: 'First Name',
    name: ADMIN.FIRST_NAME,
    ariaLabel: 'First name of the admin',
    minWidth: 60,
  },
  [ADMIN.LAST_NAME]: {
    label: 'Last Name',
    name: ADMIN.LAST_NAME,
    ariaLabel: 'Last name of the admin',
    minWidth: 60,
  },
  [ADMIN.ROLE]: {
    label: 'Role',
    name: ADMIN.ROLE,
    ariaLabel: 'Role of the admin',
    minWidth: 60,
  },
};


export const columns = mapToColumns(admin);
