import styled from 'styled-components';

interface Props {
    height: string;
}

export const ContainerContainer = styled.div<Props>`
    margin: 5px;
    background: ${(props) => props.theme.colors.bg1};
    padding: 3px;
    border-radius: 2px;
    height: ${(props) => props.height};
`;

export const ContainerHeader = styled.p`
    font-size: ${(props) => props.theme.fontSizes.small};
    color: ${(props) => props.theme.colors.fg2};
    text-align: left;
    background: ${(props) => props.theme.colors.bg1};
`;

export const ContainerContent = styled.div`
    padding: 5px;
    background: ${(props) => props.theme.colors.bg0_s};
    height: calc(100% - 23px);
    display: flex;
    flex-direction: column;
    border-radius: 5px;
`;
