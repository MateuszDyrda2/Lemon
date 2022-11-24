import styled from 'styled-components';

export const SidepanelContainer = styled.div`
    height: 100%;
    overflow: hidden;
    background: ${(props) => props.theme.colors.bg};
    display: flex;
    flex-direction: column;
`;

export const SidepanelHeader = styled.p`
    padding: 10px 0px;
    text-align: center;
    background-color: ${(props) => props.theme.colors.bg0};
    text-transform: uppercase;
    color: ${(props) => props.theme.colors.fg4};
`;

export const SidepanelContent = styled.div`
    height: 100%;
    flex: 1;
`;
