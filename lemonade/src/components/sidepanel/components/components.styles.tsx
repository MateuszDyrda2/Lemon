import styled from 'styled-components';
import { clickable, small_text } from '../../../styles/styles';

export const ComponentsContainer = styled.div`
    padding: 4px;
    height: 100%;
    width: 100%;
    overflow-y: auto;
`;

export const EntityNameContainer = styled.div`
    align-items: center;
`;

export const EntityName = styled.input`
    user-select: text;
    text-align: center;
    margin: 2px;
    box-sizing: border-box;
    border: none;
    background: ${(props) => props.theme.colors.bg_s};
    padding: 4px;
    border-radius: 2px;
    outline: none;

    ${small_text}
`;

export const IconWrapper = styled.div`
    color: ${(props) => props.theme.colors.fg1};
    border: 2px solid ${(props) => props.theme.colors.bg1};
    border-radius: 4px;
    display: flex;
    justify-content: center;

    ${clickable}
`;

export const DropdownWrapper = styled.div`
    display: block;
`;
