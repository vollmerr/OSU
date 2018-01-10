import React, { Component } from 'react';
import { Switch, Route, Redirect } from 'react-router-dom';

import Header from 'components/Header';
import routes, { home } from 'routes';

import Wrapper from './Wrapper';


class App extends Component {
    render() {
        return (
            <Wrapper>
                <Header routes={routes} />
                <Switch>
                    {
                        routes.map(route => <Route {...route} />)
                    }
                    <Redirect to={home.path} />
                </Switch>
            </Wrapper>
        );
    }
}

export default App;
