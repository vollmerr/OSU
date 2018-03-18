import React from 'react';
import ReactDOM from 'react-dom';
import { Fabric } from 'office-ui-fabric-react/lib/Fabric';
import { HashRouter } from 'react-router-dom';
import { initializeIcons } from '@uifabric/icons';

import App from './containers/App';
import registerServiceWorker from './registerServiceWorker';

// Register icons and pull the fonts from the default SharePoint cdn:
initializeIcons();

ReactDOM.render(
  <HashRouter>
    <Fabric>
      <App />
    </Fabric>
  </HashRouter>,
  document.getElementById('root')
);

registerServiceWorker();