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
import { PanelTypes } from '../../props/panel-types';
import { useContext } from 'react';
import { PanelContext, PanelContextType } from '../../state/PanelContext';

const Sidebar = () => {
    const { currentPanel, setCurrentPanel } = useContext(
        PanelContext,
    ) as PanelContextType;

    const changePanel = (newPanel: PanelTypes) =>
        setCurrentPanel(newPanel === currentPanel ? PanelTypes.None : newPanel);

    return (
        <SidebarContainer>
            <SidebarStack>
                <IconWrapper
                    onMouseDown={() => changePanel(PanelTypes.Project)}
                >
                    <LemonIcon src={LemonIco} alt="Lemon Icon" />
                </IconWrapper>
                <IconWrapper onMouseDown={() => changePanel(PanelTypes.Scene)}>
                    <VscTypeHierarchySuper size="2.5em" />
                </IconWrapper>
                <IconWrapper
                    onMouseDown={() => changePanel(PanelTypes.Components)}
                >
                    <TbComponents size="2.5em" />
                </IconWrapper>
                <IconWrapper
                    onMouseDown={() => changePanel(PanelTypes.Settings)}
                >
                    <BsGearFill size="2.5em" />
                </IconWrapper>
            </SidebarStack>
        </SidebarContainer>
    );
};

export default Sidebar;
