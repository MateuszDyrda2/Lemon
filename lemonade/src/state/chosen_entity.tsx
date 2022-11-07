import { atom } from 'recoil';

export interface Entity {
    name: string;
    id: number;
}

export const chosenEntity = atom<Entity | undefined>({
    key: 'chosenEntity',
    default: undefined,
});
