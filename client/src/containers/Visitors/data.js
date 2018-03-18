import { mapToColumns } from '../../utils/data';

import { VISITOR } from './constants';


export const visitor = {
  [VISITOR.ID]: {
    label: 'ID',
    name: VISITOR.ID,
    ariaLabel: 'ID of the role record',
    minWidth: 20,
    maxWidth: 40,
  },
  [VISITOR.FIRST_NAME]: {
    label: 'First Name',
    name: VISITOR.FIRST_NAME,
    ariaLabel: 'First Name of the visitor',
    minWidth: 60,
  },
  [VISITOR.LAST_NAME]: {
    label: 'Last Name',
    name: VISITOR.LAST_NAME,
    ariaLabel: 'Last Name of the visitor',
    minWidth: 60,
  },
  [VISITOR.BADGE_ID]: {
    label: 'Badge ID',
    name: VISITOR.BADGE_ID,
    ariaLabel: 'ID of the badge',
    minWidth: 20,
  },
  [VISITOR.VISIT_ID]: {
    label: 'Visit ID',
    name: VISITOR.VISIT_ID,
    ariaLabel: 'ID of the visit',
    minWidth: 20,
  },
};


export const columns = mapToColumns(visitor);
