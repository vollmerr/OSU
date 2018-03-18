import { mapToColumns } from '../../utils/data';

import { CAMPUS } from './constants';


export const campus = {
  [CAMPUS.ID]: {
    label: 'ID',
    name: CAMPUS.ID,
    ariaLabel: 'ID of the campus record',
    minWidth: 20,
    maxWidth: 40,
  },
  [CAMPUS.NAME]: {
    label: 'Name',
    name: CAMPUS.NAME,
    ariaLabel: 'Name of the campus',
    minWidth: 150,
  },
};


export const columns = mapToColumns(campus);
