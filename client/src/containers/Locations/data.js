import { mapToColumns } from '../../utils/data';

import { LOCATION } from './constants';


export const location = {
  [LOCATION.ID]: {
    label: 'ID',
    name: LOCATION.ID,
    ariaLabel: 'ID of the location record',
    minWidth: 20,
    maxWidth: 40,
  },
  [LOCATION.NAME]: {
    label: 'Name',
    name: LOCATION.NAME,
    ariaLabel: 'Name of the location',
    minWidth: 150,
  },
};


export const columns = mapToColumns(location);
