import { useEffect, createRef } from 'react';
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

    useEffect(() => {
        if (leftRef.current) {
            if (!width) {
                setWidth(leftRef.current?.clientWidth);
                return;
            }
            leftRef.current.style.width = `${width}px`;
        }
    }, [width, setWidth, leftRef]);

    return (
        <SidepanelContainer ref={leftRef}>
            <SidepanelHeader>Hello world</SidepanelHeader>
            <SidepanelContent />
        </SidepanelContainer>
    );
};

export default Sidepanel;
