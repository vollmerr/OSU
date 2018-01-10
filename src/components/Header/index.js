import React from 'react';

import Wrapper from './Wrapper';
import Link from './Link';


const Header = ({ routes }) => (
    <Wrapper>
        {
            routes.map(({ key, path, text }) => (
                <Link key={key} to={path}>{text}</Link>
            ))
        }
    </Wrapper>
);

export default Header;