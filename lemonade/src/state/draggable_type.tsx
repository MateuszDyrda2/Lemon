export const DraggableType = {
    ASSET: 'asset',
};

export enum AssetType {
    NONE,
    TEXTURE,
    SOUND,
    SCRIPT,
    ANIMATION,
    SHADER,
}

export interface DraggableAsset {
    assetType: AssetType;
    name: string;
    path: string;
}
