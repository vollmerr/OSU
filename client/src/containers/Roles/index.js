import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

import ErrorMessage from '../../components/ErrorMessage';
import Loading from '../../components/Loading';
import api from '../../api';
import withUtils from '../../hocs/withUtils';

import * as data from './data';
import * as C from './constants';


class Roles extends Component {
  constructor(props) {
    super(props);
    this.state = {
      roles: [],
    };
  }

  componentDidMount() {
    this.getRoles();
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
        checked: isCreating,
      },
      {
        key: 'newRandom',
        name: 'New Random',
        onClick: this.createRandomRole,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteRole,
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

  // gets list of roles from api
  getRoles = async () => {
    const { loading } = this.props;
    loading.start();
    const where = this.state.filters;
    const roles = await api.role.get({ where });
    this.setState({ roles });
    loading.stop();
  }

  createRole = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.role.create(formValues);
    await this.getRoles();
    loading.stop();
  }

  createRandomRole = async () => {
    const { loading } = this.props;
    loading.start();
    await api.role.createRandom();
    await this.getRoles();
    loading.stop();
  }

  deleteRole = async () => {
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.role.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getRoles();
    }
    loading.stop();
  }

  editRole = async () => {
    const { loading, selectedItem, formValues } = this.props;
    if (Object.values(formValues).length) {
      loading.start();
      const values = {
        ...formValues,
        id: selectedItem.id,
      };
      await api.role.edit(values);
      await this.getRoles();
      loading.stop();
    }
  }

  setFilters = async () => {
    const { formValues, filtering } = this.props;
    await this.setState({ filters: formValues });
    await this.getRoles();
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
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      [C.ROLE.NAME]: {
        label: data.role[C.ROLE.NAME].label,
        defaultValue: selectedItem[C.ROLE.NAME],
        onChanged: form.update(C.ROLE.NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editRole : isFiltering ? this.setFilters : this.createRole,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: roles,
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
            <TextField {...editProps[C.ROLE.NAME]} />
            <DefaultButton {...editProps.save} />
          </div>
        }
        {
          isLoading ?
            <Loading /> :
            <div>
              <h1>Roles</h1>
              {
                roles.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Roles.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Roles);
