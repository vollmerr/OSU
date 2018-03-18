import React from 'react';
import { NavLink } from 'react-router-dom';
import styled from 'styled-components';

import theme from '../../utils/theme';


const Wrapper = styled.header`
  display: inline-flex;
`;


const Link = styled(NavLink) `
  margin: 5px;
  padding: 10px 15px;
  text-decoration: none;
  color: ${theme.neutralPrimary};
  &.active,
  &:hover {
    color: ${theme.themeDark};
    box-shadow: 0 0 4px 1px ${(props) => theme.themePrimary};
  }
`;


const Header = ({ routes }) => (
  <Wrapper>
    {
      routes.map(({ key, path, text }) => (
        <Link exact key={key} to={path}>{text}</Link>
      ))
    }
  </Wrapper>
);


export default Header;