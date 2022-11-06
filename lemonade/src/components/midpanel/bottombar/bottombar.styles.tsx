import styled from 'styled-components';

export const BottombarContainer = styled.div`
    overflow: hidden;
    order: 0;
    background: ${(props) => props.theme.colors.bg0_h};
    display: flex;
    flex-direction: column;
`;

export const BottombarBar = styled.div`
    margin: 0 10px;
    align-items: left;
    display: flex;
`;

interface Props {
    chosen: boolean;
}
export const BottombarBarButton = styled.button<Props>`
    flex: 0 0 auto;
    font-size: ${(props) => props.theme.fontSizes.small};
    background: none;
    border: none;
    cursor: pointer;
    text-align: center;
    outline: inherit;
    padding: 15px 15px;

    color: ${(props) =>
        props.chosen ? props.theme.colors.fg0 : props.theme.colors.fg4};

    border-bottom: ${(props) =>
        props.chosen ? '2px solid ' + props.theme.colors.yellow : 'none'};

    &:hover {
        color: ${(props) => props.theme.colors.fg0};
    }
`;
