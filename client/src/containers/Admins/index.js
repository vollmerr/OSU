import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';

import Loading from '../../components/Loading';
import api from '../../api';
import withUtils from '../../hocs/withUtils';
import * as ROLES_C from '../Roles/constants';

import * as data from './data';
import * as C from './constants';


class Admins extends Component {
  constructor(props) {
    super(props);
    this.state = {
      roles: [],
      admins: [],
    };
  }

  async componentDidMount() {
    await this.getRoles();
    await this.getAdmins();
  }

  // gets list of nav items
  getNavItems = () => {
    const {
      creating,
      editing,
      isCreating,
      isEditing,
      isLoading,
      selectedItem,
      } = this.props;

    return [
      {
        key: 'new',
        name: 'New',
        onClick: isCreating ? creating.stop : creating.start,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'newRandom',
        name: 'New Random',
        onClick: this.createRandomAdmin,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteAdmin,
        disabled: isNaN(selectedItem.id) || isLoading,
        iconProps: { iconName: 'Delete' },
      },
      {
        key: 'edit',
        name: 'Edit Selected',
        onClick: isEditing ? editing.stop : editing.start,
        disabled: isNaN(selectedItem.id) || isLoading,
        iconProps: { iconName: 'Edit' },
      },
    ];
  }

  // gets list of roles from api
  getRoles = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.role.get();
    const roles = result.map((x) => ({
      key: x[ROLES_C.ROLE.ID],
      text: x[ROLES_C.ROLE.NAME],
    }));
    this.setState({ roles });
    loading.stop();
  }

  // gets list of admin from api
  getAdmins = async () => {
    const { loading } = this.props;
    loading.start();
    const admins = await api.admin.get();
    this.setState({ admins });
    loading.stop();
  }

  createAdmin = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.admin.create(formValues);
    await this.getAdmins();
    loading.stop();
  }

  createRandomAdmin = async () => {
    const { loading } = this.props;
    loading.start();
    await api.admin.createRandom();
    await this.getAdmins();
    loading.stop();
  }

  deleteAdmin = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.admin.delete(selectedItem.id);
    await this.getAdmins();
    loading.stop();
  }

  editAdmin = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.admin.delete(values);
    await this.getAdmins();
    loading.stop();
  }

  render() {
    const {
      form,
      selection,
      isCreating,
      isLoading,
      isEditing,
      selectedItem,
    } = this.props;

    if (isLoading) {
      return <Loading />;
    }

    const {
      roles,
      admins,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      firstName: {
        label: data.admin[C.ADMIN.FIRST_NAME].label,
        defaultValue: selectedItem[C.ADMIN.FIRST_NAME],
        onChanged: form.update(C.ADMIN.FIRST_NAME),
      },
      lastName: {
        label: data.admin[C.ADMIN.LAST_NAME].label,
        defaultValue: selectedItem[C.ADMIN.LAST_NAME],
        onChanged: form.update(C.ADMIN.LAST_NAME),
      },
      role: {
        label: data.admin[C.ADMIN.ROLE_NAME].label,
        defaultSelectedKey: selectedItem[C.ADMIN.ROLE_ID],
        options: roles,
        onChanged: (x) => form.update(C.ADMIN.ROLE_ID)(x.key),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editAdmin : this.createAdmin,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: admins,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <div>
        <CommandBar {...commandProps} />
        {
          (isEditing || isCreating) &&
          <div>
            <TextField {...editProps.firstName} />
            <TextField {...editProps.lastName} />
            <Dropdown {...editProps.role} />
            <DefaultButton {...editProps.save} />
          </div>
        }
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
