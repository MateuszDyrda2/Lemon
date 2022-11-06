import { createRef, useState, useEffect } from 'react';
import { ThemeProvider } from 'styled-components';
import './App.css';
import Midpanel from './components/midpanel/midpanel';
import Sidebar from './components/sidebar/sidebar';
import Sidepanel from './components/sidepanel/sidepanel';
import { PanelTypes } from './props/panel-types';
import { PanelContext } from './state/PanelContext';
import { theme } from './utils/theme';

const MIN_WIDTH = 50;

const App = () => {
    const [currentPanel, setCurrentPanel] = useState(PanelTypes.Project);
    const splitAppRef = createRef<HTMLDivElement>();
    const [sidepanelWidth, setSidepanelWidth] = useState(300);
    const [separatorXPosition, setSeparatorXPosition] = useState<
        number | undefined
    >(undefined);
    const [dragging, setDragging] = useState(false);

    const onMouseDown = (e: React.MouseEvent) => {
        setSeparatorXPosition(e.clientX);
        setDragging(true);
    };

    useEffect(() => {
        if (!dragging) return;
        const onMouseUp = (e: MouseEvent) => {
            if (!separatorXPosition) return;

            setSidepanelWidth((sidepanelWidth) => {
                const newWidth =
                    sidepanelWidth + e.clientX - separatorXPosition;
                const splitPanelWidth =
                    splitAppRef.current?.clientWidth ?? newWidth;
                return Math.min(Math.max(MIN_WIDTH, newWidth), splitPanelWidth);
            });

            setDragging(false);
        };
        window.addEventListener('mouseup', onMouseUp);
        return () => window.removeEventListener('mouseup', onMouseUp);
    }, [dragging]);

    return (
        <div className="App">
            <ThemeProvider theme={theme}>
                <PanelContext.Provider
                    value={{ currentPanel, setCurrentPanel }}
                >
                    <Sidebar />
                    <Sidepanel
                        width={sidepanelWidth}
                        setWidth={setSidepanelWidth}
                    />
                    <div
                        className="vertDiv-container"
                        onMouseDown={onMouseDown}
                    >
                        <div className="vertDiv" />
                    </div>
                    <Midpanel />
                </PanelContext.Provider>
            </ThemeProvider>
        </div>
    );
};

export default App;
