import {
    SidebarContainer,
    SidebarStack,
    IconWrapper,
    LemonIcon,
} from './sidebar.styles';

import { BsGearFill } from 'react-icons/bs';
import { TbComponents } from 'react-icons/tb';
import { VscTypeHierarchySuper } from 'react-icons/vsc';
import LemonIco from '../../img/lemon.svg';

const Sidebar = () => {
    return (
        <SidebarContainer>
            <SidebarStack>
                <IconWrapper>
                    <LemonIcon src={LemonIco} alt="Lemon Icon" />
                </IconWrapper>
                <IconWrapper>
                    <VscTypeHierarchySuper size="2.5em" />
                </IconWrapper>
                <IconWrapper>
                    <TbComponents size="2.5em" />
                </IconWrapper>
                <IconWrapper>
                    <BsGearFill size="2.5em" />
                </IconWrapper>
            </SidebarStack>
        </SidebarContainer>
    );
};

export default Sidebar;
