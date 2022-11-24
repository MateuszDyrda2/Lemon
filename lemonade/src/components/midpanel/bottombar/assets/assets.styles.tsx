import styled from 'styled-components';
import { clickable, small_text } from '../../../../styles/styles';

export const AssetsContainer = styled.div`
    width: 100%;
    height: 100%;
    display: flex;
    flex-flow: column wrap;
    align-items: flex-start;
`;

export const IconWrapper = styled.div`
    text-align: center;
    color: ${(props) => props.theme.colors.fg2};
    cursor: pointer;
    margin: 20px;

    &:hover {
        color: ${(props) => props.theme.colors.fg0};
    }
    &:active {
        color: ${(props) => props.theme.colors.fg1};
    }
`;

export const FolderName = styled.p`
    ${small_text}
`;
