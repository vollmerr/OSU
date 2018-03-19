import React from 'react';
import { Selection } from 'office-ui-fabric-react/lib/DetailsList';


const withUtils = (C) => class extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      errorMessage: null,
      isCreating: false,
      isLoading: 0,
      isEditing: false,
      selectedItem: {},
      formValues: {},
    };
    // keeps track of selected item in list
    this.selection = new Selection({
      onSelectionChanged: this.handleSelectionChange,
    });
  }

  setError = (errorMessage) => {
    this.setState({ errorMessage });
  }

  clearError = () => {
    this.setState({ errorMessage: null });
  }

  // increment api call in progress
  startLoading = () => {
    this.handleClearSelection();
    this.setState({
      isCreating: false,
      isLoading: this.state.isLoading + 1,
      isEditing: false,
    });
  }

  // decrement api call in progress
  stopLoading = () => {
    this.setState({ isLoading: this.state.isLoading - 1 });
    // fix detailsList scrolling bug.....
    window.dispatchEvent(new Event('resize'));
  }

  startEditing = () => {
    this.setState({ isEditing: true, isCreating: false });
  }

  stopEditing = () => {
    this.setState({ isEditing: false });
  }

  startCreating = () => {
    this.handleClearSelection();
    this.setState({ isCreating: true, isEditing: false });
  }

  stopCreating = () => {
    this.setState({ isCreating: false });
  }

  updateForm = (key) => (value) => {
    this.setState({ formValues: { ...this.state.formValues, [key]: value } });
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
    const creating = {
      start: this.startCreating,
      stop: this.stopCreating,
    };

    const error = {
      setError: this.setError,
      clear: this.clearError,
    };

    const loading = {
      start: this.startLoading,
      stop: this.stopLoading,
    };

    const editing = {
      start: this.startEditing,
      stop: this.stopEditing,
    };

    const form = {
      update: this.updateForm,
    };

    const props = {
      form,
      error,
      creating,
      loading,
      editing,
      selection: this.selection,
      ...this.state,
      ...this.props,
    }

    return <C {...props} />
  }
};


export default withUtils;

