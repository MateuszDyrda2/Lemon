import styled from 'styled-components';

export const ProjectContainer = styled.div`
    height: 100%;
`;

export const LemonImg = styled.img`
    margin: 20px;
`;

export const ButtonGroup = styled.ul`
    list-style-type: none;
    align-items: center;
    text-align: center;
    display: flex;
    flex-direction: column;
`;

export const ProjectName = styled.p`
    color: ${(props) => props.theme.colors.fg0};
    font-size: ${(props) => props.theme.fontSizes.medium};
    padding: 15px;
`;

export const Button = styled.li`
    display: inline-block;
    width: 10rem;
    cursor: pointer;
    border-radius: 2px;
    margin: 3px;
    padding: 5px;
    border: 1px solid ${(props) => props.theme.colors.fg4};
    &:hover {
        background: ${(props) => props.theme.colors.bg3};
    }
    &:active {
        background: ${(props) => props.theme.colors.fg4};
    }
`;
