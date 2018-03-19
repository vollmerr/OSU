import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { DatePicker } from 'office-ui-fabric-react';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';

import ErrorMessage from '../../components/ErrorMessage';
import Loading from '../../components/Loading';
import api from '../../api';
import withUtils from '../../hocs/withUtils';
import * as CL_C from '../CampusLocations/constants';

import * as data from './data';
import * as C from './constants';


class Visits extends Component {
  constructor(props) {
    super(props);
    this.state = {
      visits: [],
      campusLocations: [],
    };
  }

  async componentDidMount() {
    await this.getVisits();
    await this.getCampusLocations();
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
        onClick: this.createRandomVisit,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteVisit,
        disabled: isNaN(selectedItem.id) || isLoading,
        iconProps: { iconName: 'Delete' },
      },
      {
        key: 'edit',
        name: 'Edit Selected',
        onClick: (x) => {
          this.setState({ campusSelected: isEditing });
          return isEditing ? editing.stop(x) : editing.start(x);
        },
        disabled: isNaN(selectedItem.id) || isLoading,
        iconProps: { iconName: 'Edit' },
      },
    ];
  }

  // gets list of locations from api for dropdown
  getCampusLocations = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.campusLocation.get({});
    const campusLocations = result.map((x) => ({
      key: x[CL_C.CAMPUS_LOCATION.ID],
      text: `${x[CL_C.CAMPUS_LOCATION.CAMPUS_NAME]} - ${x[CL_C.CAMPUS_LOCATION.LOCATION_NAME]}`,
    }));
    this.setState({ campusLocations });
    loading.stop();
  }

  // gets list of visits from api
  getVisits = async () => {
    const { loading } = this.props;
    loading.start();
    const visits = await api.visit.get();
    this.setState({ visits });
    loading.stop();
  }

  createVisit = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.visit.create(formValues);
    await this.getVisits();
    loading.stop();
  }

  createRandomVisit = async () => {
    const { loading } = this.props;
    loading.start();
    await api.visit.createRandom();
    await this.getVisits();
    loading.stop();
  }

  deleteVisit = async () => {
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.visit.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getVisits();
    }
    loading.stop();
  }

  editVisit = async () => {
    const { loading, selectedItem, formValues } = this.props;
    if (Object.values(formValues).length) {
      loading.start();
      const values = {
        ...formValues,
        id: selectedItem.id,
      };
      await api.visit.edit(values);
      await this.getVisits();
      loading.stop();
    }
  }

  render() {
    const {
      form,
      error,
      title,
      formValues,
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
      visits,
      campusLocations,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      [C.VISIT.DATE]: {
        label: data.visit[C.VISIT.DATE].label,
        defaultValue: selectedItem[C.VISIT.DATE],
        value: formValues[C.VISIT.DATE],
        onSelectDate: form.update(C.VISIT.DATE),
      },
      [C.VISIT.CAMPUS_LOCATION_ID]: {
        label: 'Campus - Location',
        defaultSelectedKey: selectedItem[C.VISIT.CAMPUS_LOCATION_ID],
        options: campusLocations,
        onChanged: (x) => form.update(C.VISIT.CAMPUS_LOCATION_ID)(x.key),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editVisit : this.createVisit,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: visits,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <div>
        <CommandBar {...commandProps} />
        {
          (isEditing || isCreating) &&
          <div>
            {title && <h3>{title}</h3>}
            <DatePicker {...editProps[C.VISIT.DATE]} />
            <Dropdown {...editProps[C.VISIT.CAMPUS_LOCATION_ID]} />
            <DefaultButton {...editProps.save} />
          </div>
        }
        {
          isLoading ?
            <Loading /> :
            <div>
              <h1>Visits</h1>
              {
                visits.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Visits.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Visits);
