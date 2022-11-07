import styled from 'styled-components';

export const SceneContainer = styled.div`
    height: 100%;
    width: 100%;
`;

export const EntityList = styled.div`
    overflow-y: auto;
    position: relative;
`;

interface WrapperProps {
    selected: boolean;
}
export const EntityWrapper = styled.div<WrapperProps>`
    position: relative;
    overflow: hidden;
    margin: 0 10px;

    background: ${(props) =>
        props.selected ? props.theme.colors.bg1 : props.theme.colors.bg0};
    &:hover {
        background: ${(props) => props.theme.colors.bg2};
    }
    &:active {
        background: ${(props) => props.theme.colors.bg3};
    }
`;

export const EntityName = styled.p`
    margin: 0;
    color: ${(props) => props.theme.colors.fg1};
    font-size ${(props) => props.theme.fontSizes.small};
`;
