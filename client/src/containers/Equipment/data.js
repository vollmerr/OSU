import { mapToColumns } from '../../utils/data';

import { EQUIPMENT } from './constants';


export const equipment = {
  [EQUIPMENT.ID]: {
    label: 'ID',
    name: EQUIPMENT.ID,
    ariaLabel: 'ID of the equipment',
    minWidth: 20,
    maxWidth: 40,
  },
  [EQUIPMENT.NAME]: {
    label: 'Name',
    name: EQUIPMENT.NAME,
    ariaLabel: 'Name of the equipment',
    minWidth: 150,
  },
};

export const columns = mapToColumns(equipment);