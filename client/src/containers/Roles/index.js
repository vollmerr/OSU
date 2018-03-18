import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

import Loading from '../../components/Loading';
import * as api from '../../api/role';
import withUtils from '../../hocs/withUtils';

import * as data from './data';
import * as C from './constants';


class Roles extends Component {
  constructor(props) {
    super(props);
    this.state = {
      roles: [],
    };

    this.values = {};
  }

  componentDidMount() {
    this.getRoles();
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
    ];
  }

  // gets list of roles from api
  getRoles = async () => {
    const { loading } = this.props;
    loading.start();
    const roles = await api.getRoles();
    this.setState({ roles });
    loading.stop();
  }

  createRole = async () => {
    const { loading } = this.props;
    loading.start();
    const values = Object.keys(this.values).reduce((x, k) => ({
      ...x,
      [k]: this.values[k].value,
    }), {});
    await api.createRole(values);
    await this.getRoles();
    loading.stop();
  }

  createRandomRole = async () => {
    const { loading } = this.props;
    loading.start();
    await api.createRandomRole();
    await this.getRoles();
    loading.stop();
  }

  deleteRole = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.deleteRole(selectedItem.id);
    await this.getRoles();
    loading.stop();
  }

  editRole = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    const values = Object.keys(this.values).reduce((x, k) => ({
      ...x,
      [k]: this.values[k].value,
    }), { id: selectedItem.id });
    await api.editRole(values);
    await this.getRoles();
    loading.stop();
  }

  render() {
    const {
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
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        componentRef: (x) => { this.values[C.ROLE.NAME] = x; },
        label: data.role[C.ROLE.NAME].label,
        value: selectedItem[C.ROLE.NAME],
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editRole : this.createRole,
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
          (isEditing || isCreating) &&
          <div>
            <TextField {...editProps.name} />
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
