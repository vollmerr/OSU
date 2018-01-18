import styled from 'styled-components';

import theme from '../../utils/theme';


const A = styled.a`
    position: absolute;
    top: 0;
    right: 0;
    padding: 10px;

    &:hover {
        > svg {
            fill: ${theme.themePrimary};
        }
    }
`;

export default A;