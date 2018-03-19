import React, { Component } from 'react';
import {
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { TextField } from 'office-ui-fabric-react/lib/TextField';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { Dropdown } from 'office-ui-fabric-react/lib/Dropdown';

import Loading from '../../components/Loading';
import api from '../../api';
import withUtils from '../../hocs/withUtils';
import * as V_C from '../Visits/constants';
import * as BT_C from '../BadgeTypes/constants';

import * as data from './data';
import * as C from './constants';


class Visitors extends Component {
  constructor(props) {
    super(props);
    this.state = {
      visitors: [],
      badgeTypes: [],
      visits: [],
    };
  }

  async componentDidMount() {
    await this.getVisitors();
    await this.getBadgeTypes();
    await this.getVisits();
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

  getBadgeTypes = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.badgeType.get({});
    const badgeTypes = result.map((x) => ({
      key: x[BT_C.BADGE_TYPE.ID],
      text: x[BT_C.BADGE_TYPE.TYPE],
    }));
    this.setState({ badgeTypes });
    loading.stop();
  }

  // gets list of locations from api for dropdown
  getVisits = async () => {
    const { loading } = this.props;
    loading.start();
    const result = await api.visit.get({});
    const visits = result.map((x) => ({
      key: x[V_C.VISIT.ID],
      text: `${x[V_C.VISIT.DATE]} at ${x[V_C.VISIT.CAMPUS_NAME]} - ${x[V_C.VISIT.LOCATION_NAME]}`,
    }));
    this.setState({ visits });
    loading.stop();
  }

  // gets list of visitors from api
  getVisitors = async () => {
    const { loading } = this.props;
    loading.start();
    const visitors = await api.visitor.get({});
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
      badgeTypes,
      visits,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const editProps = {
      [C.VISITOR.FIRST_NAME]: {
        label: data.visitor[C.VISITOR.FIRST_NAME].label,
        defaultValue: selectedItem[C.VISITOR.FIRST_NAME],
        onChanged: form.update(C.VISITOR.FIRST_NAME),
      },
      [C.VISITOR.LAST_NAME]: {
        label: data.visitor[C.VISITOR.LAST_NAME].label,
        defaultValue: selectedItem[C.VISITOR.LAST_NAME],
        onChanged: form.update(C.VISITOR.LAST_NAME),
      },
      [C.VISITOR.BADGE_TYPE]: {
        label: data.visitor[C.VISITOR.BADGE_TYPE].label,
        defaultSelectedKey: selectedItem[C.VISITOR.BADGE_ID],
        options: badgeTypes,
        onChanged: (x) => form.update(C.VISITOR.BADGE_ID)(x.key),
      },
      [C.VISITOR.VISIT_ID]: {
        label: 'Visit',
        defaultSelectedKey: selectedItem[C.VISITOR.VISIT_ID],
        options: visits,
        onChanged: (x) => form.update(C.VISITOR.VISIT_ID)(x.key),
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
            <TextField {...editProps[C.VISITOR.FIRST_NAME]} />
            <TextField {...editProps[C.VISITOR.LAST_NAME]} />
            <Dropdown {...editProps[C.VISITOR.BADGE_TYPE]} />
            <Dropdown {...editProps[C.VISITOR.VISIT_ID]} />
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
