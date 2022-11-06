import styled from 'styled-components';

export const TopbarContainer = styled.div`
    background: ${(props) => props.theme.colors.bg0};
    height: 38px;
    border-bottom: 1px solid ${(props) => props.theme.colors.bg3};
    order: 3;
    align-items: center;
`;
