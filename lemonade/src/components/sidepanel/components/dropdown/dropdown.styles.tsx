import styled from 'styled-components';

import { clickable } from '../../../../styles/styles';

interface ContainerProps {
    isOpen: boolean;
}
export const DropdownContainer = styled.div<ContainerProps>`
    display: ${(props) => (props.isOpen ? 'inline-block' : 'none')};
    width: 100%;
`;

export const DropdownList = styled.ul`
    list-style: none;
    align-items: center;
    border: 2px solid ${(props) => props.theme.colors.bg2};
    border-radius: 0 0 3px 3px;
`;

export const DropdownItem = styled.li`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    border: 1px solid ${(props) => props.theme.colors.bg2};
    padding: 3px 0px;

    ${clickable}
`;
