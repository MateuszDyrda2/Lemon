import styled from 'styled-components';

export const MidpanelContainer = styled.div`
    display: flex;
    flex-flow: column nowrap;
    flex: 1;
    order: 3;
    width: 100%;
    background: ${(props) => props.theme.colors.bg1};
`;
