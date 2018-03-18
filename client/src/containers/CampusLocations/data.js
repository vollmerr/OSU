import { mapToColumns } from '../../utils/data';

import { CAMPUS_LOCATION } from './constants';


export const campusLocation = {
  [CAMPUS_LOCATION.ID]: {
    label: 'ID',
    name: CAMPUS_LOCATION.ID,
    ariaLabel: 'ID of the campus location record',
    minWidth: 20,
    maxWidth: 40,
  },
  [CAMPUS_LOCATION.CAMPUS_ID]: {
    label: 'Campus ID',
    name: CAMPUS_LOCATION.CAMPUS_ID,
    ariaLabel: 'ID of the campus',
    minWidth: 20,
  },
  [CAMPUS_LOCATION.CAMPUS_NAME]: {
    label: 'Campus Name',
    name: CAMPUS_LOCATION.CAMPUS_NAME,
    ariaLabel: 'Name of the campus',
    minWidth: 60,
  },
  [CAMPUS_LOCATION.LOCATION_ID]: {
    label: 'Location ID',
    name: CAMPUS_LOCATION.LOCATION_ID,
    ariaLabel: 'ID of the location',
    minWidth: 20,
  },
  [CAMPUS_LOCATION.LOCATION_NAME]: {
    label: 'Location Name',
    name: CAMPUS_LOCATION.LOCATION_NAME,
    ariaLabel: 'Name of the location',
    minWidth: 60,
  },
};


export const columns = mapToColumns(campusLocation);
