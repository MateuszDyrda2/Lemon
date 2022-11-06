import { createRef, useEffect, useState } from 'react';

import Scenes from './scenes/scenes';
import Assets from './assets/assets';
import Output from './output/output';

import {
    BottombarBar,
    BottombarBarButton,
    BottombarContainer,
} from './bottombar.styles';

interface Props {
    height: number;
    setHeight: React.Dispatch<React.SetStateAction<number>>;
}

enum TabTypes {
    Scenes,
    Assets,
    Output,
}

const Bottombar = ({ height, setHeight }: Props) => {
    const bottomRef = createRef<HTMLDivElement>();
    const [currentTab, setCurrentTab] = useState<TabTypes>(TabTypes.Scenes);

    useEffect(() => {
        if (bottomRef.current) {
            if (!height) {
                setHeight(bottomRef.current.clientHeight);
                return;
            }
            bottomRef.current.style.height = `${height}px`;
        }
    }, [height, setHeight, bottomRef]);

    const onClick = (type: TabTypes) => {
        setCurrentTab(type);
    };

    return (
        <BottombarContainer ref={bottomRef}>
            <BottombarBar>
                <BottombarBarButton
                    chosen={currentTab === TabTypes.Scenes}
                    onMouseDown={() => onClick(TabTypes.Scenes)}
                >
                    Scenes
                </BottombarBarButton>
                <BottombarBarButton
                    chosen={currentTab === TabTypes.Assets}
                    onMouseDown={() => onClick(TabTypes.Assets)}
                >
                    Assets
                </BottombarBarButton>
                <BottombarBarButton
                    chosen={currentTab === TabTypes.Output}
                    onMouseDown={() => onClick(TabTypes.Output)}
                >
                    Output
                </BottombarBarButton>
            </BottombarBar>
            {(currentTab == TabTypes.Scenes && <Scenes />) ||
                (currentTab == TabTypes.Assets && <Assets />) ||
                (currentTab == TabTypes.Output && <Output />)}
        </BottombarContainer>
    );
};

export default Bottombar;
