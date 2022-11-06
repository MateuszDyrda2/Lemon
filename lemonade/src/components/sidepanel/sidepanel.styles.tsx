import styled from 'styled-components';

export const SidepanelContainer = styled.div`
    height: 100%;
    overflow: hidden;
    background-color: ${(props) => props.theme.colors.bg};
`;

export const SidepanelHeader = styled.p`
    padding: 10px 0px;
    text-align: center;
    background-color: ${(props) => props.theme.colors.bg0};
    text-transform: uppercase;
    color: ${(props) => props.theme.colors.fg4};
    border-bottom: 1px solid ${(props) => props.theme.colors.bg3};
`;

export const SidepanelContent = styled.div``;
