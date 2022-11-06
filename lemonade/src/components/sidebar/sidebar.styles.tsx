import styled from 'styled-components';

export const SidebarContainer = styled.div`
    float: left;
    min-width: 56px;
    order: 0;
    background-color: ${(props) => props.theme.colors.bg};
    border-right: 1px solid ${(props) => props.theme.colors.bg2};
`;

export const SidebarStack = styled.div`
    height: 100vh;
    align-items: center;
    display: flex;
    flex-direction: column;
    margin: 1rem 0px;
`;

export const IconWrapper = styled.div`
    text-align: center;
    padding: 5px 0;
    color: ${(props) => props.theme.colors.fg2};
    &:hover {
        color: ${(props) => props.theme.colors.fg0};
    }
    cursor: pointer;
`;

export const LemonIcon = styled.img`
    width: 3rem;
    transition: transform 0.4s ease-in-out;

    &:hover {
        transform: rotate(90deg);
    }
`;
