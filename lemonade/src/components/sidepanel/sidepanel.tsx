import { useEffect, createRef, useContext } from 'react';
import { PanelTypes } from '../../props/panel-types';
import { PanelContext, PanelContextType } from '../../state/PanelContext';
import Components from './components/components';
import Project from './project/project';
import Scene from './scene/scene';
import Settings from './settings/settings';
import {
    SidepanelContainer,
    SidepanelHeader,
    SidepanelContent,
} from './sidepanel.styles';

interface Props {
    width: number;
    setWidth: React.Dispatch<React.SetStateAction<number>>;
}

const Sidepanel = ({ width, setWidth }: Props) => {
    const leftRef = createRef<HTMLDivElement>();
    const { currentPanel } = useContext(PanelContext) as PanelContextType;

    useEffect(() => {
        if (leftRef.current) {
            if (!width) {
                setWidth(leftRef.current?.clientWidth);
                return;
            }
            const cw = currentPanel === PanelTypes.None ? 0 : width;
            leftRef.current.style.width = `${cw}px`;
        }
    }, [width, setWidth, leftRef]);

    return (
        <SidepanelContainer ref={leftRef}>
            <SidepanelHeader>{currentPanel}</SidepanelHeader>
            <SidepanelContent>
                {(currentPanel === PanelTypes.Project && <Project />) ||
                    (currentPanel === PanelTypes.Scene && <Scene />) ||
                    (currentPanel === PanelTypes.Components && (
                        <Components />
                    )) ||
                    (currentPanel === PanelTypes.Settings && <Settings />) || (
                        <></>
                    )}
            </SidepanelContent>
        </SidepanelContainer>
    );
};

export default Sidepanel;
