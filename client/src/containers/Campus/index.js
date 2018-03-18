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


class Campus extends Component {
  constructor(props) {
    super(props);
    this.state = {
      campus: [],
    };
  }

  componentDidMount() {
    this.getCampus();
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
        onClick: this.createRandomCampus,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteCampus,
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

  // gets list of campus from api
  getCampus = async () => {
    const { loading } = this.props;
    loading.start();
    const campus = await api.campus.get();
    this.setState({ campus });
    loading.stop();
  }

  createCampus = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.campus.create(formValues);
    await this.getCampus();
    loading.stop();
  }

  createRandomCampus = async () => {
    const { loading } = this.props;
    loading.start();
    await api.campus.createRandom();
    await this.getCampus();
    loading.stop();
  }

  deleteCampus = async () => {
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.campus.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getCampus();
    }
    loading.stop();
  }

  editCampus = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.campus.edit(values);
    await this.getCampus();
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
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        label: data.campus[C.CAMPUS.NAME].label,
        defaultValue: selectedItem[C.CAMPUS.NAME],
        onChanged: form.update(C.CAMPUS.NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editCampus : this.createCampus,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: campus,
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
              <h1>Campus</h1>
              {
                campus.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Campus.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Campus);
