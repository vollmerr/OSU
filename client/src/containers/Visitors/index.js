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


class Visitors extends Component {
  constructor(props) {
    super(props);
    this.state = {
      visitors: [],
    };
  }

  componentDidMount() {
    this.getVisitors();
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
        onClick: this.createRandomVisitor,
        disabled: isLoading,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteVisitor,
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

  // gets list of visitors from api
  getVisitors = async () => {
    const { loading } = this.props;
    loading.start();
    const visitors = await api.visitor.get();
    this.setState({ visitors });
    loading.stop();
  }

  createVisitor = async () => {
    const { loading, formValues } = this.props;
    loading.start();
    await api.visitor.create(formValues);
    await this.getVisitors();
    loading.stop();
  }

  createRandomVisitor = async () => {
    const { loading } = this.props;
    loading.start();
    await api.visitor.createRandom();
    await this.getVisitors();
    loading.stop();
  }

  deleteVisitor = async () => {
    const { loading, selectedItem } = this.props;
    loading.start();
    await api.visitor.delete(selectedItem.id);
    await this.getVisitors();
    loading.stop();
  }

  editVisitor = async () => {
    const { loading, selectedItem, formValues } = this.props;
    loading.start();
    const values = {
      ...formValues,
      id: selectedItem.id,
    };
    await api.visitor.edit(values);
    await this.getVisitors();
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
      visitors,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      name: {
        label: data.visitor[C.VISITOR.FIRST_NAME].label,
        defaultValue: selectedItem[C.VISITOR.FIRST_NAME],
        onChanged: form.update(C.VISITOR.FIRST_NAME),
      },
      save: {
        text: 'Save',
        primary: true,
        onClick: isEditing ? this.editVisitor : this.createVisitor,
      }
    }

    const listProps = {
      selection,
      columns: data.columns,
      items: visitors,
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
              <h1>Visitors</h1>
              {
                visitors.length ?
                  <DetailsList {...listProps} /> :
                  <div>No Visitors.....</div>
              }
            </div>
        }
      </div>
    );
  }
}


export default withUtils(Visitors);
