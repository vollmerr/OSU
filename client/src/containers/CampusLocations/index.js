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
import * as C_C from '../Campus/constants';
import * as L_C from '../Locations/constants';

import * as data from './data';
import * as C from './constants';


class CampusLocations extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: null,
      campus: [],
      locations: [],
      campusLocations: [],
    };
  }

  async componentDidMount() {
    await this.getCampusLocations();
    await this.getCampus();
    await this.getLocations();
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

  // gets list of locations from api for dropdown
  getCampus = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.campus.get({});
    const campus = result.map((x) => ({
      key: x[C_C.CAMPUS.ID],
      text: x[C_C.CAMPUS.NAME],
    }));
    this.setState({ campus });
    loading.stop();
  }

  // gets list of locations from api for dropdown
  getLocations = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.location.get({});
    const locations = result.map((x) => ({
      key: x[L_C.LOCATION.ID],
      text: x[L_C.LOCATION.NAME],
    }));
    this.setState({ locations });
    loading.stop();
  }

  // gets list of campusLocations from api
  getCampusLocations = async () => {
    const { loading } = this.props;
    loading.start();
    const campusLocations = await api.campusLocation.get({});
    this.setState({ campusLocations });
    loading.stop();
  }

  createCampusLocation = async () => {
    const { loading, formValues, error } = this.props;
    loading.start();
    const response = await api.campusLocation.create(formValues);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getCampusLocations();
    }
    loading.stop();
  }

  createRandomCampusLocation = async () => {
    const { loading, error } = this.props;
    loading.start();
    const response = await api.campusLocation.createRandom();
    if (response.status === 400) {
      error.setError(await response.json());
    } else if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
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
    const { loading, selectedItem, formValues, error } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    const response = await api.campusLocation.edit(values);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getCampusLocations();
    }
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
      campus,
      locations,
      campusLocations,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      [C.CAMPUS_LOCATION.CAMPUS_NAME]: {
        label: data.campusLocation[C.CAMPUS_LOCATION.CAMPUS_NAME].label,
        defaultSelectedKey: selectedItem[C.CAMPUS_LOCATION.CAMPUS_ID],
        options: campus,
        onChanged: (x) => form.update(C.CAMPUS_LOCATION.CAMPUS_ID)(x.key),
      },
      [C.CAMPUS_LOCATION.LOCATION_NAME]: {
        label: data.campusLocation[C.CAMPUS_LOCATION.LOCATION_NAME].label,
        defaultSelectedKey: selectedItem[C.CAMPUS_LOCATION.LOCATION_ID],
        options: locations,
        onChanged: (x) => form.update(C.CAMPUS_LOCATION.LOCATION_ID)(x.key),
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
            <Dropdown {...editProps[C.CAMPUS_LOCATION.CAMPUS_NAME]} />
            <Dropdown {...editProps[C.CAMPUS_LOCATION.LOCATION_NAME]} />
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
