import styled from 'styled-components';

export const MidpanelContainer = styled.div`
    display: flex;
    flex-direction: column;
    flex: 1;
    background: ${(props) => props.theme.colors.bg1};
`;

export const DividerContainer = styled.div`
    cursor: ns-resize;
    height: 5px;
    margin-bottom: -5px;
    position: relative;
`;

export const Divider = styled.div`
    position: absolute;
    top: 0px;
    left: 0px;
    bottom: auto;
    z-index: 100;
    display: block;
    height: 2px;
`;
