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


class CampusLocations extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: null,
      campusLocations: [],
    };
  }

  componentDidMount() {
    this.getCampusLocations();
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
        checked: isCreating,
      },
      {
        key: 'newRandom',
        name: 'New Random',
        onClick: this.createRandomCampusLocation,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteCampusLocation,
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

  // gets list of campusLocations from api
  getCampusLocations = async () => {
    const { loading } = this.props;
    loading.start();
    const campusLocations = await api.campusLocation.get();
    this.setState({ campusLocations });
    loading.stop();
  }

  createCampusLocation = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.campusLocation.create(formValues);
    await this.getCampusLocations();
    loading.stop();
  }

  createRandomCampusLocation = async () => {
    const { loading, error } = this.props;
    loading.start();
    const response = await api.campusLocation.createRandom();
    if (response.status === 400) {
      error.setError(await response.json());
    } else {
      await this.getCampusLocations();
    }
    loading.stop();
  }

  deleteCampusLocation = async () => {
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.campusLocation.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getCampusLocations();
    }
    loading.stop();
  }

  editCampusLocation = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.campusLocation.edit(values);
    await this.getCampusLocations();
    loading.stop();
  }

  render() {
    const {
      form,
      error,
      selection,
      isCreating,
      isLoading,
      isEditing,
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
      campusLocations,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      campusName: {
        label: data.campusLocation[C.CAMPUS_LOCATION.CAMPUS_NAME].label,
        defaultValue: selectedItem[C.CAMPUS_LOCATION.CAMPUS_NAME],
        onChanged: form.update(C.CAMPUS_LOCATION.CAMPUS_NAME),
      },
      locationName: {
        label: data.campusLocation[C.CAMPUS_LOCATION.LOCATION_NAME].label,
        defaultValue: selectedItem[C.CAMPUS_LOCATION.LOCATION_NAME],
        onChanged: form.update(C.CAMPUS_LOCATION.LOCATION_NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editCampusLocation : this.createCampusLocation,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: campusLocations,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <div>
        <CommandBar {...commandProps} />
        {
          (isEditing || isCreating) &&
          <div>
            <TextField {...editProps.campusName} />
            <TextField {...editProps.locationName} />
            <DefaultButton {...editProps.save} />
          </div>
        }
        {
          isLoading ?
            <Loading /> :
            <div>
              <h1>Campus-Locations</h1>
              {
                campusLocations.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Campus-Locations.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(CampusLocations);
