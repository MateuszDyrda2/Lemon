import { useRef, useState, useEffect, createRef } from 'react';

import Bottombar from './bottombar/bottombar';
import {
    DividerContainer,
    Divider,
    MidpanelContainer,
} from './midpanel.styles';
import Topbar from './topbar/topbar';
import Viewport from './viewport/viewport';

const MIN_HEIGHT = 50;

const Midpanel = () => {
    const splitAppRef = createRef<HTMLDivElement>();
    const [height, setHeight] = useState(300);
    const [separatorPos, setSeparatorPos] = useState<number | undefined>(
        undefined,
    );
    const [dragging, setDragging] = useState(false);

    const onMouseDown = (e: React.MouseEvent) => {
        setSeparatorPos(e.clientY);
        setDragging(true);
    };

    useEffect(() => {
        if (!dragging) return;
        const onMouseUp = (e: MouseEvent) => {
            if (!separatorPos) return;

            setHeight((height) => {
                const newHeight = height - e.clientY + separatorPos;
                const sHeight = splitAppRef.current?.clientHeight ?? newHeight;
                return Math.min(Math.max(MIN_HEIGHT, newHeight), newHeight);
            });

            setDragging(false);
        };
        window.addEventListener('mouseup', onMouseUp);
        return () => window.removeEventListener('mouseup', onMouseUp);
    }, [dragging]);

    return (
        <MidpanelContainer>
            <Topbar />
            <DividerContainer onMouseDown={onMouseDown}>
                <Divider />
            </DividerContainer>
            <Bottombar height={height} setHeight={setHeight} />
        </MidpanelContainer>
    );
};

export default Midpanel;
