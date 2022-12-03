import { useState, useEffect } from 'react';
import { RecoilRoot } from 'recoil';
import { ThemeProvider } from 'styled-components';
import './App.css';
import Midpanel from './components/midpanel/midpanel';
import Sidebar from './components/sidebar/sidebar';
import Sidepanel from './components/sidepanel/sidepanel';
import { PanelTypes } from './props/panel-types';
import { PanelContext } from './state/PanelContext';
import { theme } from './utils/theme';
import { registerAll } from '@tauri-apps/api/globalShortcut';
import { invoke } from '@tauri-apps/api';
import { DndProvider } from 'react-dnd';
import { HTML5Backend } from 'react-dnd-html5-backend';

const MIN_WIDTH = 200;

const App = () => {
    const [currentPanel, setCurrentPanel] = useState(PanelTypes.Project);
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

                return Math.max(MIN_WIDTH, newWidth);
            });

            setDragging(false);
        };
        window.addEventListener('mouseup', onMouseUp);
        return () => window.removeEventListener('mouseup', onMouseUp);
    }, [dragging]);

    useEffect(() => {
        registerAll(['CommandOrCtrl+s', 'CommandOrCtrl+r'], (shortcut) => {
            if (shortcut === 'CommandOrCtrl+s') {
                invoke('save').catch(console.error);
            } else if (shortcut === 'CommandOrCtrl+r') {
                invoke('recreate_assets').catch(console.error);
            }
        }).catch(console.error);
    }, []);

    return (
        <RecoilRoot>
            <div className="App">
                <ThemeProvider theme={theme}>
                    <PanelContext.Provider
                        value={{ currentPanel, setCurrentPanel }}
                    >
                        <DndProvider backend={HTML5Backend}>
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
                        </DndProvider>
                    </PanelContext.Provider>
                </ThemeProvider>
            </div>
        </RecoilRoot>
    );
};

export default App;
