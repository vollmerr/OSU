import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';

import Loading from '../../components/Loading';
import api from '../../api';
import withUtils from '../../hocs/withUtils';

import * as data from './data';
import * as C from './constants';


class Equipment extends Component {
  constructor(props) {
    super(props);
    this.state = {
      equipment: [],
    };
  }

  componentDidMount() {
    this.getEquipment();
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
        onClick: this.createRandomEquipment,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteEquipment,
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

  // gets list of equipment from api
  getEquipment = async () => {
    const { loading } = this.props;
    loading.start();
    const equipment = await api.equipment.get();
    this.setState({ equipment });
    loading.stop();
  }

  createEquipment = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.equipment.create(formValues);
    await this.getEquipment();
    loading.stop();
  }

  createRandomEquipment = async () => {
    const { loading } = this.props;
    loading.start();
    await api.equipment.createRandom();
    await this.getEquipment();
    loading.stop();
  }

  deleteEquipment = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.equipment.delete(selectedItem.id);
    await this.getEquipment();
    loading.stop();
  }

  editEquipment = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.equipment.edit(values);
    await this.getEquipment();
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
      equipment,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        label: data.equipment[C.EQUIPMENT.NAME].label,
        defaultValue: selectedItem[C.EQUIPMENT.NAME],
        onChanged: form.update(C.EQUIPMENT.NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editEquipment : this.createEquipment,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: equipment,
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
              <h1>Equipment</h1>
              {
                equipment.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Equipment.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Equipment);
