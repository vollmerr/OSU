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


class Visits extends Component {
  constructor(props) {
    super(props);
    this.state = {
      visits: [],
    };
  }

  componentDidMount() {
    this.getVisits();
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
        onClick: isEditing ? editing.stop : editing.start,
        disabled: isNaN(selectedItem.id) || isLoading,
        iconProps: { iconName: 'Edit' },
      },
    ];
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
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.visit.delete(selectedItem.id);
    await this.getVisits();
    loading.stop();
  }

  editVisit = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.visit.edit(values);
    await this.getVisits();
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
      visits,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        label: data.visit[C.VISIT.FIRST_NAME].label,
        defaultValue: selectedItem[C.VISIT.FIRST_NAME],
        onChanged: form.update(C.VISIT.FIRST_NAME),
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
            <TextField {...editProps.name} />
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
