import {
    DropdownContainer,
    DropdownItem,
    DropdownList,
} from './dropdown.styles';

interface Props {
    isOpen: boolean;
    options: string[];
    onSelect: { (option: string): void };
}

const Dropdown = ({ isOpen, options, onSelect }: Props) => {
    return (
        <DropdownContainer isOpen={isOpen}>
            <DropdownList>
                {options.map((opt, index) => (
                    <DropdownItem key={index} onClick={() => onSelect(opt)}>
                        {opt}
                    </DropdownItem>
                ))}
            </DropdownList>
        </DropdownContainer>
    );
};

export default Dropdown;
