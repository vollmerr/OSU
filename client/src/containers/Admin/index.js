// import React, { Component } from 'react';
// import {
//   DetailsList,
//   SelectionMode,
// } from 'office-ui-fabric-react/lib/DetailsList';
// import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
// import { TextField } from 'office-ui-fabric-react/lib/TextField';
// import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

// import Loading from '../../components/Loading';
// import * as api from '../../api/equipment';
// import withUtils from '../../hocs/withUtils';

// import * as data from './data';
// import * as C from './constants';


// class Equipment extends Component {
//   constructor(props) {
//     super(props);
//     this.state = {
//       equipment: [],
//     };

//     this.values = {
//       [C.ADMIN.ID]: null,
//     };
//   }

//   componentDidMount() {
//     this.getEquipment();
//   }

//   // gets list of nav items
//   getNavItems = () => {
//     const {
//       creating,
//       editing,
//       isCreating,
//       isEditing,
//       isLoading,
//       selectedItem,
//     } = this.props;

//     return [
//       {
//         key: 'new',
//         name: 'New',
//         onClick: isCreating ? creating.stop : creating.start,
//         disabled: isLoading,
//         iconProps: { iconName: 'Add' },
//       },
//       {
//         key: 'newRandom',
//         name: 'New Random',
//         onClick: this.createRandomEquipment,
//         disabled: isLoading,
//         iconProps: { iconName: 'Add' },
//       },
//       {
//         key: 'delete',
//         name: 'Delete Selected',
//         onClick: this.deleteEquipment,
//         disabled: isNaN(selectedItem.id) || isLoading,
//         iconProps: { iconName: 'Delete' },
//       },
//       {
//         key: 'edit',
//         name: 'Edit Selected',
//         onClick: isEditing ? editing.stop : editing.start,
//         disabled: isNaN(selectedItem.id) || isLoading,
//         iconProps: { iconName: 'Edit' },
//       },
//     ];
//   }

//   // gets list of equipment from api
//   getEquipment = async () => {
//     const { loading } = this.props;
//     loading.start();
//     const equipment = await api.getEquipment();
//     this.setState({ equipment });
//     loading.stop();
//   }

//   createEquipment = async () => {
//     const { loading } = this.props;
//     loading.start();
//     const values = Object.keys(this.values).reduce((x, k) => ({
//       ...x,
//       [k]: this.values[k].value,
//     }), {});
//     await api.createEquipment(values);
//     await this.getEquipment();
//     loading.stop();
//   }

//   createRandomEquipment = async () => {
//     const { loading } = this.props;
//     loading.start();
//     await api.createRandomEquipment();
//     await this.getEquipment();
//     loading.stop();
//   }

//   deleteEquipment = async () => {
//     const { loading, selectedItem, selection } = this.props;
//     loading.start();
//     await api.deleteEquipment(selectedItem.id);
//     await this.getEquipment();
//     selection.clear();
//     loading.stop();
//   }

//   editEquipment = async () => {
//     const { loading, selectedItem, selection } = this.props;
//     loading.start();
//     const values = Object.keys(this.values).reduce((x, k) => ({
//       ...x,
//       [k]: this.values[k].value,
//     }), { id: selectedItem.id });
//     await api.editEquipment(values);
//     await this.getEquipment();
//     selection.clear();
//     loading.stop();
//   }

//   render() {
//     const {
//       selection,
//       isCreating,
//       isLoading,
//       isEditing,
//       selectedItem,
//     } = this.props;

//     if (isLoading) {
//       return <Loading />;
//     }

//     const {
//       equipment,
//     } = this.state;

//     const commandProps = {
//       items: this.getNavItems(),
//     };

//     const editProps = {
//       name: {
//         componentRef: (x) => { this.values[C.EQUIPMENT.NAME] = x; },
//         label: data.equipment[C.EQUIPMENT.NAME].label,
//         value: selectedItem[C.EQUIPMENT.NAME],
//       },
//       save: {
//         text: 'Save',
//         primary: true,
//         onClick: isEditing ? this.editEquipment : this.createEquipment,
//       }
//     }

//     const listProps = {
//       columns: data.columns,
//       items: equipment,
//       selection: selection.tracker,
//       selectionMode: SelectionMode.single,
//       selectionPreservedOnEmptyClick: true,
//     };

//     return (
//       <div>
//         <CommandBar {...commandProps} />
//         {
//           (isEditing || isCreating) &&
//           <div>
//             <TextField {...editProps.name} />
//             <DefaultButton {...editProps.save} />
//           </div>
//         }
//         {
//           isLoading ?
//             <Loading /> :
//             <div>
//               <h1>Equipment</h1>
//               {
//                 equipment.length ?
//                   <DetailsList {...listProps} /> :
//                   <div>No Equipment.....</div>
//               }
//             </div>
//         }
//       </div>
//     );
//   }
// }


// export default withUtils(Equipment);