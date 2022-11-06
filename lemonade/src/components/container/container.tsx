import {
    ContainerContainer,
    ContainerContent,
    ContainerHeader,
} from './container.styles';

interface Props {
    header: string;
    children?: JSX.Element | JSX.Element[];
}

const Container = ({ header, children }: Props) => {
    return (
        <ContainerContainer>
            <ContainerHeader>{header}</ContainerHeader>
            <ContainerContent>{children}</ContainerContent>
        </ContainerContainer>
    );
};

export default Container;
