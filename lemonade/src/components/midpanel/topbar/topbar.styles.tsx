import styled from 'styled-components';

export const TopbarContainer = styled.div`
    background: ${(props) => props.theme.colors.bg0};
    height: 38px;
    align-items: center;
    display: flex;
    position: relative;
    flex-direction: row;
    justify-content: center;
`;

const IconContainer = styled.div`
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
`;

export const ToolboxContainer = styled(IconContainer)`
    position: absolute;
    left: 0;
    border-radius: 5px;
    margin: 5px;
    background: ${(props) => props.theme.colors.bg1};
`;

interface ToolProps {
    isActive: boolean;
}

export const ToolWrapper = styled.div<ToolProps>`
    text-align: center;
    justify-content: center;
    align-items: center;
    padding 0 5px;
    color: ${(props) =>
        props.isActive ? props.theme.colors.fg0 : props.theme.colors.fg3};
    &:hover {
        color: ${(props) => props.theme.colors.fg0};
    }
    cursor: pointer;
`;

export const PlayContainer = styled(IconContainer)``;

export const PlayWrapper = styled.div`
    padding: 0 5px;
    text-align: center;
    align-items: center;
    color: ${(props) => props.theme.colors.fg2};
    &:hover {
        color: ${(props) => props.theme.colors.fg0};
    }
    cursor: pointer;
`;
