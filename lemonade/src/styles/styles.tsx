import { css } from 'styled-components';

export const clickable = css`
    &:hover {
        background: ${(props) => props.theme.colors.bg2};
    }
    &:active {
        background: ${(props) => props.theme.colors.bg1};
    }

    cursor: pointer;
`;

export const small_text = css`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    text-overflow: ellipsis;
`;

export const medium_text = css`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.medium};
    text-overflow: ellipsis;
`;

export const large_text = css`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.large};
    text-overflow: ellipsis;
`;

export const flex_column = css`
    display: flex;
    flex-flow: column nowrap;
`;

export const flex_row = css`
    display: flex;
    flex-flow: row nowrap;
`;
