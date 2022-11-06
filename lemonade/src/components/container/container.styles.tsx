import styled from 'styled-components';

export const ContainerContainer = styled.div`
    margin: 5px;
    background: ${(props) => props.theme.colors.bg2};
    padding: 5px;
    border-radius: 5px;
`;

export const ContainerHeader = styled.p`
    font-size: ${(props) => props.theme.colors.fg2};
    text-align: left;
    background: ${(props) => props.theme.colors.bg2};
`;

export const ContainerContent = styled.div`
    background: ${(props) => props.theme.colors.bg0};
    display: flex;
    flex-direction: column;
    border-radius: 5px;
`;
