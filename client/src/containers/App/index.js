
import React, { Component } from 'react';
import { Switch, Route, Redirect } from 'react-router-dom';
import styled from 'styled-components';

import GitHubLogo from '../../components/GitHubLogo';
import Header from '../../components/Header';
import theme from '../../utils/theme';

import routes, { home } from './routes';


const Wrapper = styled.div`
  margin: auto;
  padding: 0 15px;
  @media (min-width: ${theme.breakPoints.md}px) {
    width: ${Math.floor(theme.breakPoints.md * 0.9)}px;
  }
  @media (min-width: ${theme.breakPoints.lg}px) {
    width: ${Math.floor(theme.breakPoints.lg * 0.8)}px;
  }
  @media (min-width: ${theme.breakPoints.xl}px) {
    width: ${Math.floor(theme.breakPoints.xl * 0.7)}px;
  }
  .ms-CommandBar {
    background: transparent;
  }
`;


class App extends Component {
  render() {
    return (
      <Wrapper>
        <GitHubLogo />
        <Header routes={routes} />
        <Switch>
          {routes.map(route => <Route {...route} />)}
          <Redirect to={home.path} />
        </Switch>
      </Wrapper>
    );
  }
}


export default App;