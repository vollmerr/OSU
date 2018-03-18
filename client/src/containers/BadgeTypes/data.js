import { mapToColumns } from '../../utils/data';

import { BADGE_TYPE } from './constants';


export const badgeType = {
  [BADGE_TYPE.ID]: {
    label: 'ID',
    name: BADGE_TYPE.ID,
    ariaLabel: 'ID of the badge type record',
    minWidth: 20,
    maxWidth: 40,
  },
  [BADGE_TYPE.TYPE]: {
    label: 'Name',
    name: BADGE_TYPE.TYPE,
    ariaLabel: 'Type of badge',
    minWidth: 150,
  },
};


export const columns = mapToColumns(badgeType);
