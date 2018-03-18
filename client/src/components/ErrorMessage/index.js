import React from 'react';
import styled from 'styled-components';
import { DefaultButton } from 'office-ui-fabric-react';

import theme from '../../utils/theme';


const Message = styled.p`
    color: ${theme.red};
`;

const ErrorMessage = ({ message, onClick }) => (
  <div>
    <Message>{message}</Message>
    <DefaultButton text={'Ok'} onClick={onClick} />
  </div>
)


export default ErrorMessage;
