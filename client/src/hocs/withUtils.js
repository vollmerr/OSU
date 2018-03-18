import React from 'react';
import { Selection } from 'office-ui-fabric-react/lib/DetailsList';


const withUtils = (C) => class extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            isCreating: false,
            isLoading: 0,
            isEditing: false,
            selectedItem: {},
        };
        // keeps track of equipment
        this.selection = new Selection({
            onSelectionChanged: this.handleSelectionChange,
        });
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
        this.setState({ isEditing: true });
    }

    stopEditing = () => {
        this.setState({ isEditing: false });
    }

    startCreating = () => {
        this.handleClearSelection();
        this.setState({ isCreating: true });
    }

    stopCreating = () => {
        this.setState({ isCreating: false });
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

        const loading = {
            start: this.startLoading,
            stop: this.stopLoading,
        };

        const editing = {
            start: this.startEditing,
            stop: this.stopEditing,
        };
        
        const props = {
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

