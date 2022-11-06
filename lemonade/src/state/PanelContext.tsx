import { createContext } from 'react';
import { PanelTypes } from '../props/panel-types';

export interface PanelContextType {
    currentPanel: PanelTypes;
    setCurrentPanel: React.Dispatch<React.SetStateAction<PanelTypes>>;
}

export const PanelContext = createContext<PanelContextType | undefined>(
    undefined,
);
