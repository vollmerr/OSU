import { mapToColumns } from '../../utils/data';

import { VISITOR } from './constants';


export const visitor = {
  [VISITOR.ID]: {
    label: 'ID',
    name: VISITOR.ID,
    ariaLabel: 'ID of the role record',
  },
  [VISITOR.FIRST_NAME]: {
    label: 'First Name',
    name: VISITOR.FIRST_NAME,
    ariaLabel: 'First Name of the visitor',
  },
  [VISITOR.LAST_NAME]: {
    label: 'Last Name',
    name: VISITOR.LAST_NAME,
    ariaLabel: 'Last Name of the visitor',
  },
  [VISITOR.BADGE_ID]: {
    label: 'Badge ID',
    name: VISITOR.BADGE_ID,
    ariaLabel: 'ID of the badge',
  },
  [VISITOR.BADGE_TYPE]: {
    label: 'Badge Type',
    name: VISITOR.BADGE_TYPE,
    ariaLabel: 'Type of badge',
  },
  [VISITOR.VISIT_ID]: {
    label: 'Visit ID',
    name: VISITOR.VISIT_ID,
    ariaLabel: 'ID of the visit',
  },
  [VISITOR.VISIT_DATE]: {
    label: 'Visit Date',
    name: VISITOR.VISIT_DATE,
    ariaLabel: 'Date of visit',
  },
  [VISITOR.CAMPUS_ID]: {
    label: 'Campus ID',
    name: VISITOR.CAMPUS_ID,
    ariaLabel: 'ID of campus',
  },
  [VISITOR.CAMPUS_NAME]: {
    label: 'Campus Name',
    name: VISITOR.CAMPUS_NAME,
    ariaLabel: 'Name of campus',
  },
  [VISITOR.LOCATION_ID]: {
    label: 'Location ID',
    name: VISITOR.LOCATION_ID,
    ariaLabel: 'ID of Location',
  },
  [VISITOR.LOCATION_NAME]: {
    label: 'Location Name',
    name: VISITOR.LOCATION_NAME,
    ariaLabel: 'Name of Location',
  },
  [VISITOR.CAMPUS_LOCATION_ID]: {
    label: 'Campus-Location ID',
    name: VISITOR.CAMPUS_LOCATION_ID,
    ariaLabel: 'ID of Campus-Location',
  }
};

const fields = [
  VISITOR.ID,
  VISITOR.FIRST_NAME,
  VISITOR.LAST_NAME,
  VISITOR.BADGE_TYPE,
  VISITOR.VISIT_DATE,
  VISITOR.CAMPUS_NAME,
  VISITOR.LOCATION_NAME,
];
export const columns = mapToColumns(visitor, fields);
