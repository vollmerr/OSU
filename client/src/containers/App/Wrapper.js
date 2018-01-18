import styled from 'styled-components';

import theme from '../../utils/theme';


const Wrapper = styled.div`
    display: flex;
    flex-direction: column;     
    min-height: 600px;
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

export default Wrapper;