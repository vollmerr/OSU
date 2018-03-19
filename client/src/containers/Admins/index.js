import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';

import ErrorMessage from '../../components/ErrorMessage';
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
      filtering,
      isCreating,
      isEditing,
      isLoading,
      isFiltering,
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
      {
        key: 'filter',
        name: 'Filters',
        onClick: isFiltering ? filtering.stop : filtering.start,
        iconProps: { iconName: 'Search' },
      },
    ];
  }

  // gets list of roles from api for dropdown
  getRoles = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.role.get({});
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
    const where = this.state.filters;
    const admins = await api.admin.get({ where });
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
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.admin.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
    await this.getAdmins();
    }
    loading.stop();
  }

  editAdmin = async () => {
    const { loading, selectedItem, formValues } = this.props;
    if (Object.values(formValues).length) {
      loading.start();
      const values = {
        ...formValues,
        id: selectedItem.id,
      };
      await api.admin.edit(values);
      await this.getAdmins();
      loading.stop();
    }
  }

  setFilters = async () => {
    const { formValues, filtering } = this.props;
    await this.setState({ filters: formValues });
    await this.getAdmins();
    filtering.stop();
  }

  render() {
    const {
      form,
      error,
      title,
      selection,
      isCreating,
      isLoading,
      isEditing,
      isFiltering,
      selectedItem,
      errorMessage,
    } = this.props;

    if (errorMessage) {
      return <ErrorMessage message={errorMessage} onClick={error.clear} />;
    }

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
      [C.ADMIN.FIRST_NAME]: {
        label: data.admin[C.ADMIN.FIRST_NAME].label,
        defaultValue: selectedItem[C.ADMIN.FIRST_NAME],
        onChanged: form.update(C.ADMIN.FIRST_NAME),
      },
      [C.ADMIN.LAST_NAME]: {
        label: data.admin[C.ADMIN.LAST_NAME].label,
        defaultValue: selectedItem[C.ADMIN.LAST_NAME],
        onChanged: form.update(C.ADMIN.LAST_NAME),
      },
      [C.ADMIN.ROLE_NAME]: {
        label: data.admin[C.ADMIN.ROLE_NAME].label,
        defaultSelectedKey: selectedItem[C.ADMIN.ROLE_ID],
        options: roles,
        onChanged: (x) => form.update(C.ADMIN.ROLE_ID)(x.key),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editAdmin : isFiltering ? this.setFilters : this.createAdmin,
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
          (isEditing || isCreating || isFiltering) &&
          <div>
            {title && <h3>{title}</h3>}
            <TextField {...editProps[C.ADMIN.FIRST_NAME]} />
            <TextField {...editProps[C.ADMIN.LAST_NAME]} />
            <Dropdown {...editProps[C.ADMIN.ROLE_NAME]} />
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
