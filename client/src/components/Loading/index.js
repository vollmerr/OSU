import styled from 'styled-components';
import {
    Spinner,
    SpinnerSize,
} from 'office-ui-fabric-react/lib/Spinner';


const Loading = styled(Spinner) `
    margin: auto;
`;

Loading.defaultProps = {
    label: 'Loading...',
    ariaLive: 'assertive',
    size: SpinnerSize.large,
};


export default Loading;