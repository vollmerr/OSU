import React, { Component } from 'react';
import { CommandBar } from 'office-ui-fabric-react/lib/CommandBar';
import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import {
  Selection,
  DetailsList,
  SelectionMode,
} from 'office-ui-fabric-react/lib/DetailsList';

import Loading from 'components/Loading';

import Wrapper from './Wrapper';
import Content from './Content';


const columns = [
  {
    key: 'id',
    fieldName: 'id',
    name: 'Id',
    minWidth: 20,
    maxWidth: 20,
  },
  {
    key: 'username',
    fieldName: 'username',
    name: 'Username',
    minWidth: 150,
  },
  {
    key: 'password',
    fieldName: 'password',
    name: 'Password',
    minWidth: 150,
  },
  {
    key: 'created_at',
    fieldName: 'created_at',
    name: 'Created At',
    minWidth: 150,
  },
];


class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      users: [],
      isLoading: 0,
      selectedItem: {},
    };
    // keeps track of
    this.selection = new Selection({
      onSelectionChanged: this.handleSelectionChange,
    });
  }

  componentDidMount() {
    this.getUsers();
  }

  // gets list of nav items
  getNavItems = () => {
    return [
      {
        key: 'new',
        name: 'New Random',
        onClick: this.newRandomUser,
        iconProps: { iconName: 'Add' },
      },
      {
        key: 'delete',
        name: 'Delete Selected',
        onClick: this.deleteUser,
        disabled: isNaN(this.state.selectedItem.id),
        iconProps: { iconName: 'Delete' },
      },
    ];
  }

  // gets list of users from api
  getUsers = async () => {
    this.startRequest();
    await fetch('/users')
      .then(res => res.json())
      .then(users => (
        this.setState({ users })
      ));
    this.endRequest();
  }

  // uses api to create a new random user
  newRandomUser = async () => {
    const options = {
      method: 'post',
    };

    this.startRequest();
    await fetch('/users/create', options);
    await this.getUsers();
    this.endRequest();
  }

  // uses api to delete the selected user
  deleteUser = async () => {
    const id = this.state.selectedItem.id;
    const options = {
      method: 'post',
    };

    this.startRequest();
    await fetch(`/users/${id}/delete`, options);
    await this.getUsers();
    this.handleClearSelection();
    this.endRequest();
  }

  // increment api call in progress  
  startRequest = () => {
    this.setState({ isLoading: this.state.isLoading + 1 });
  }

  // decrement api call in progress
  endRequest = () => {
    this.setState({ isLoading: this.state.isLoading - 1 });
    // fix detailsList scrolling bug.....
    window.dispatchEvent(new Event('resize'));
  }

  // handles selecting an item from the list
  handleSelectionChange = () => {
    const selectedItems = this.selection.getSelection()
    if (selectedItems.length) {
      this.setState({
        selectedItem: selectedItems[0],
      });
    }
  }

  // handles clearing the selection from the list
  handleClearSelection = () => {
    const index = this.selection.getSelectedIndices()[0];
    this.selection.toggleKeySelected(index);
    this.setState({ selectedItem: {} });
  }

  render() {
    const {
      users,
      isLoading,
      selectedItem,
    } = this.state;

    const commandProps = {
      items: this.getNavItems(),
    };

    const listProps = {
      columns,
      items: users,
      selection: this.selection,
      selectionMode: SelectionMode.single,
      selectionPreservedOnEmptyClick: true,
    };

    return (
      <Wrapper>
        {
          isLoading ?
            <Loading /> :
            <Content>
              <CommandBar {...commandProps} />

              <h1>Test Users</h1>
              {
                users.length ?
                  <DetailsList {...listProps} /> :
                  <div>No test users.....</div>
              }
            </Content>
        }
      </Wrapper>
    );
  }
}


export default App;