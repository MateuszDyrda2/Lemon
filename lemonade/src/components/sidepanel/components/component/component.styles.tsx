import { MdArrowDropDown, MdArrowDropUp } from 'react-icons/md';
import styled from 'styled-components';
import {
    small_text,
    medium_text,
    flex_column,
} from '../../../../styles/styles';

export const ComponentClass = styled.div`
    border: 1px solid ${(props) => props.theme.colors.bg1};
    margin: 10px 0;
    ${flex_column}
    border-radius: 2px;
`;
export const ComponentHeader = styled.div`
    background: ${(props) => props.theme.colors.bg1};
    align-items: center;
    display: flex;
    cursor: pointer;
`;
export const ComponentName = styled.p`
    ${small_text}
    padding: 2px 10px 2px 10px;
    margin: 0;
    overflow: hidden;
`;

export const DropDown = styled(MdArrowDropDown)`
    ${medium_text}
    margin-left: 5px;
`;

export const DropUp = styled(MdArrowDropUp)`
    ${medium_text}
    margin-left: 5px;
`;
