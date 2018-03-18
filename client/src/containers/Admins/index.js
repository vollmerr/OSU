import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

import Loading from '../../components/Loading';
import * as api from '../../api/admin';
import withUtils from '../../hocs/withUtils';

import * as data from './data';
import * as C from './constants';


class Admins extends Component {
  constructor(props) {
    super(props);
    this.state = {
      admins: [],
    };

    // this.values = {
    //   [C.EQUIPMENT.NAME]: null,
    // };
  }

  componentDidMount() {
    this.getAdmins();
  }

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
  //         onClick: this.createRandomAdmin,
  //         disabled: isLoading,
  //         iconProps: { iconName: 'Add' },
  //       },
  //       {
  //         key: 'delete',
  //         name: 'Delete Selected',
  //         onClick: this.deleteAdmin,
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

  // gets list of admin from api
  getAdmins = async () => {
    const { loading } = this.props;
    loading.start();
    const admins = await api.getAdmins();
    this.setState({ admins });
    loading.stop();
  }

  //   createAdmin = async () => {
  //     const { loading } = this.props;
  //     loading.start();
  //     const values = Object.keys(this.values).reduce((x, k) => ({
  //       ...x,
  //       [k]: this.values[k].value,
  //     }), {});
  //     await api.createAdmin(values);
  //     await this.getAdmin();
  //     loading.stop();
  //   }

    createRandomAdmin = async () => {
      const { loading } = this.props;
      loading.start();
      await api.createRandomAdmin();
      await this.getAdmins();
      loading.stop();
    }

  //   deleteAdmin = async () => {
  //     const { loading, selectedItem } = this.props;
  //     loading.start();
  //     await api.deleteAdmin(selectedItem.id);
  //     await this.getAdmin();
  //     loading.stop();
  //   }

  //   editAdmin = async () => {
  //     const { loading, selectedItem } = this.props;
  //     loading.start();
  //     const values = Object.keys(this.values).reduce((x, k) => ({
  //       ...x,
  //       [k]: this.values[k].value,
  //     }), { id: selectedItem.id });
  //     await api.editAdmin(values);
  //     await this.getAdmin();
  //     loading.stop();
  //   }

  render() {
    const {
      selection,
      //   isCreating,
      isLoading,
      //   isEditing,
      selectedItem,
    } = this.props;

    if (isLoading) {
      return <Loading />;
    }

    const {
      admins,
    } = this.state;

    // const commandProps = {
    //   items: this.getNavItems(),
    // };

    // const editProps = {
    //   name: {
    //     componentRef: (x) => { this.values[C.EQUIPMENT.NAME] = x; },
    //     label: data.admin[C.EQUIPMENT.NAME].label,
    //     value: selectedItem[C.EQUIPMENT.NAME],
    //   },
    //   save: {
    //     text: 'Save',
    //     primary: true,
    //     onClick: isEditing ? this.editAdmin : this.createAdmin,
    //   }
    // }

    const listProps = {
      selection,
      columns: data.columns,
      items: admins,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <div>
        {/* <CommandBar {...commandProps} /> */}
        {/* {
          (isEditing || isCreating) &&
          <div>
            <TextField {...editProps.name} />
            <DefaultButton {...editProps.save} />
          </div>
        } */}
        {
          isLoading ?
            <Loading /> :
            <div>
              <h1>Admins</h1>
              {
                admins.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Admins.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Admins);
