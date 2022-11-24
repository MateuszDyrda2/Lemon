import { atom } from 'recoil';

export interface Entity {
    id: number;
}

export const chosenEntity = atom<Entity | undefined>({
    key: 'chosenEntity',
    default: undefined,
});
