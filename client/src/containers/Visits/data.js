import { mapToColumns } from '../../utils/data';

import { VISIT } from './constants';


export const visit = {
  [VISIT.ID]: {
    label: 'ID',
    name: VISIT.ID,
    ariaLabel: 'ID of the role record',
    minWidth: 20,
    maxWidth: 40,
  },
  [VISIT.DATE]: {
    label: 'Visit Date',
    name: VISIT.DATE,
    ariaLabel: 'Date of the visit',
    minWidth: 60,
  },
  [VISIT.CAMPUS_ID]: {
    label: 'Campus ID',
    name: VISIT.CAMPUS_ID,
    ariaLabel: 'ID of the campus record',
    minWidth: 20,
  },
  [VISIT.LOCATION_ID]: {
    label: 'Location ID',
    name: VISIT.LOCATION_ID,
    ariaLabel: 'ID of the location',
    minWidth: 20,
  },
};


export const columns = mapToColumns(visit);
