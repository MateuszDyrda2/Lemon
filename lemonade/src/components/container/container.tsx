import {
    ContainerContainer,
    ContainerContent,
    ContainerHeader,
} from './container.styles';

interface Props {
    header: string;
    children?: JSX.Element | JSX.Element[];
    height: string;
}

const Container = ({ header, children, height }: Props) => {
    return (
        <ContainerContainer height={height}>
            <ContainerHeader>{header}</ContainerHeader>
            <ContainerContent>{children}</ContainerContent>
        </ContainerContainer>
    );
};

export default Container;
