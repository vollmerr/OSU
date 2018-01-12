import styled from 'styled-components';


const Wrapper = styled.div`
    display: flex;
    flex-direction: column;     
    min-height: 600px;
    margin: auto;
    width: 300px;

    @media (min-width: 768px) {
        width: 600px;
    }

    @media (min-width: 1224px) {
        width: 800px;
    }

    .ms-CommandBar {
        background: transparent;
    }
`;

export default Wrapper;