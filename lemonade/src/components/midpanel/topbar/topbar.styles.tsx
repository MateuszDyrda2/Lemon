import styled from 'styled-components';

export const TopbarContainer = styled.div`
    background: ${(props) => props.theme.colors.bg0};
    height: 38px;
    width: 100%;
    border-bottom: 1px solid ${(props) => props.theme.colors.bg3};
    order: 0;
    align-items: center;
`;
