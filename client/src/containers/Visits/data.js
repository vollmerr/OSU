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
    maxWidth: 60,
  },
  [VISIT.CAMPUS_NAME]: {
    label: 'Campus Name',
    name: VISIT.CAMPUS_NAME,
    ariaLabel: 'Name of the campus',
    minWidth: 60,
  },
  [VISIT.LOCATION_ID]: {
    label: 'Location ID',
    name: VISIT.LOCATION_ID,
    ariaLabel: 'ID of the location record',
    minWidth: 20,
    maxWidth: 60,
  },
  [VISIT.LOCATION_NAME]: {
    label: 'Location Name',
    name: VISIT.LOCATION_NAME,
    ariaLabel: 'Name of the location',
    minWidth: 60,
  },
};


export const columns = mapToColumns(visit);
