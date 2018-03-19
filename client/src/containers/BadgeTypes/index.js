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


class BadgeTypes extends Component {
  constructor(props) {
    super(props);
    this.state = {
      badgeTypes: [],
    };
  }

  componentDidMount() {
    this.getBadgeTypes();
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
        onClick: this.createRandomBadgeType,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteBadgeType,
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

  // gets list of badgeTypes from api
  getBadgeTypes = async () => {
    const { loading } = this.props;
    loading.start();
    const badgeTypes = await api.badgeType.get({});
    this.setState({ badgeTypes });
    loading.stop();
  }

  createBadgeType = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.badgeType.create(formValues);
    await this.getBadgeTypes();
    loading.stop();
  }

  createRandomBadgeType = async () => {
    const { loading } = this.props;
    loading.start();
    await api.badgeType.createRandom();
    await this.getBadgeTypes();
    loading.stop();
  }

  deleteBadgeType = async () => {
    const { loading, selectedItem, error } = this.props;
    loading.start();
    const response = await api.badgeType.delete(selectedItem.id);
    if (response.status === 500) {
      const message = await response.json();
      error.setError(message.sqlMessage);
    } else {
      await this.getBadgeTypes();
    }
    loading.stop();
  }

  editBadgeType = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.badgeType.edit(values);
    await this.getBadgeTypes();
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
      badgeTypes,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      [C.BADGE_TYPE.TYPE]: {
        label: data.badgeType[C.BADGE_TYPE.TYPE].label,
        defaultValue: selectedItem[C.BADGE_TYPE.TYPE],
        onChanged: form.update(C.BADGE_TYPE.TYPE),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editBadgeType : this.createBadgeType,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: badgeTypes,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <div>
        <CommandBar {...commandProps} />
        {
          (isEditing || isCreating) &&
          <div>
            <TextField {...editProps[C.BADGE_TYPE.TYPE]} />
            <DefaultButton {...editProps.save} />
          </div>
        }
        {
          isLoading ?
            <Loading /> :
            <div>
              <h1>Badge Types</h1>
              {
                badgeTypes.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Badge Types.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(BadgeTypes);
