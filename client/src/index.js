import React from 'react';
import ReactDOM from 'react-dom';
import App from './containers/App';
import registerServiceWorker from './registerServiceWorker';
import { Fabric } from 'office-ui-fabric-react/lib/Fabric';
import { initializeIcons } from '@uifabric/icons';

// Register icons and pull the fonts from the default SharePoint cdn:
initializeIcons();

ReactDOM.render(
    <Fabric>
        <App />
    </Fabric>,
    document.getElementById('root')
);

registerServiceWorker();
