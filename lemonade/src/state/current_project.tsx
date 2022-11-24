import { atom } from 'recoil';

interface Project {
    name: string;
}

export const current_project = atom<Project | undefined>({
    key: 'loadedProject',
    default: undefined,
});
