import { atom } from 'recoil';
import { Tools } from '../props/tools';

export interface SelectedTools {
    tool: Tools;
    grid: boolean;
}

export const selectedTools = atom<SelectedTools>({
    key: 'selectedTools',
    default: { tool: Tools.Mouse, grid: false },
});
