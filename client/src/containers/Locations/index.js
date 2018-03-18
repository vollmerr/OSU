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


class Locations extends Component {
  constructor(props) {
    super(props);
    this.state = {
      locations: [],
    };
  }

  componentDidMount() {
    this.getLocations();
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
        checked : isCreating,
      },
      {
        key: 'newRandom',
        name: 'New Random',
        onClick: this.createRandomLocation,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteLocation,
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

  // gets list of locations from api
  getLocations = async () => {
    const { loading } = this.props;
    loading.start();
    const locations = await api.location.get();
    this.setState({ locations });
    loading.stop();
  }

  createLocation = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.location.create(formValues);
    await this.getLocations();
    loading.stop();
  }

  createRandomLocation = async () => {
    const { loading } = this.props;
    loading.start();
    await api.location.createRandom();
    await this.getLocations();
    loading.stop();
  }

  deleteLocation = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.location.delete(selectedItem.id);
    await this.getLocations();
    loading.stop();
  }

  editLocation = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.location.edit(values);
    await this.getLocations();
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
      locations,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        label: data.location[C.LOCATION.NAME].label,
        value: selectedItem[C.LOCATION.NAME],
        onChanged: form.update(C.LOCATION.NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editLocation : this.createLocation,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: locations,
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
              <h1>Locations</h1>
              {
                locations.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Locations.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Locations);
