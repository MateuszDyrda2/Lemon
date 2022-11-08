import { MdArrowDropDown, MdArrowDropUp } from 'react-icons/md';
import styled from 'styled-components';

export const ObjectInput = styled.input`
    margin: 2px;
    width: 8rem;
    box-sizing: border-box;
    border: none;
    background: ${(props) => props.theme.colors.bg_s};
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    padding: 4px;
    border-radius: 2px;
    outline: none;
    user-select: text;
`;
export const ObjectClass = styled.div`
    display: inline;
`;
export const FieldClass = styled.div`
    overflow: hidden;
`;
export const FieldValues = styled.div`
    margin: 10px 20px;
    overflow: hidden;
    float: right;
`;
export const FieldName = styled.p`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    margin: 10px 20px;
    float: left;
`;

export const ComponentClass = styled.div`
    border: 1px solid ${(props) => props.theme.colors.bg1};
    margin: 10px 0;
    display: flex;
    flex-direction: column;
    border-radius: 2px;
`;
export const ComponentHeader = styled.div`
    background: ${(props) => props.theme.colors.bg1};
    align-items: center;
    display: flex;
    cursor: pointer;
`;
export const ComponentName = styled.p`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    padding: 2px 10px 2px 10px;
    margin: 0;
    text-overflow: ellipsis;
    overflow: hidden;
`;

export const DropDown = styled(MdArrowDropDown)`
    font-size: large;
    margin-left: 5px;
`;

export const DropUp = styled(MdArrowDropUp)`
    font-size: large;
    margin-left: 5px;
`;

export const ComponentsContainer = styled.div`
    padding: 4px;
    height: 100%;
    width: 100%;
    overflow-y: auto;
`;
