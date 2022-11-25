import styled from 'styled-components';
import { small_text } from '../../../../styles/styles';

export const ObjectInput = styled.input`
    margin: 2px;
    width: 8rem;
    box-sizing: border-box;
    border: none;
    background: ${(props) => props.theme.colors.bg_s};
    padding: 4px;
    border-radius: 2px;
    outline: none;
    user-select: text;
    ${small_text}
`;
export const ObjectClass = styled.div`
    display: inline;
`;
export const FieldClass = styled.div`
    overflow: hidden;

    border-bottom: 1px solid ${(props) => props.theme.colors.bg1};
`;
export const FieldValues = styled.div`
    margin: 10px 20px;
    overflow: hidden;
    float: right;
`;
export const FieldName = styled.p`
    color: ${(props) => props.theme.colors.fg1};
    font-size: ${(props) => props.theme.fontSizes.small};
    margin: 10px 20px;
    float: left;
`;
