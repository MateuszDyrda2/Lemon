import { AssetsContainer, IconWrapper, FolderName } from './assets.styles';
import { FaFolder } from 'react-icons/fa';
import { AiFillFile } from 'react-icons/ai';
import { useState } from 'react';
import { invoke } from '@tauri-apps/api';
import { useDrag } from 'react-dnd';
import { AssetType, DraggableType } from '../../../../state/draggable_type';

const directories = ['textures', 'sounds', 'scripts', 'animations', 'shaders'];

interface AssetModel {
    name: string;
    path: string;
}

interface AssetProps {
    name: string;
    path: string;
    type: AssetType;
}

const Asset = ({ name, path, type }: AssetProps) => {
    const [{ isDragging }, drag] = useDrag(() => ({
        type: DraggableType.ASSET,
        item: { name: name, path: path, type: type },
        collect: (monitor) => ({
            isDragging: !!monitor.isDragging(),
        }),
    }));

    return (
        <IconWrapper ref={drag}>
            <AiFillFile size="4em" />
            <FolderName>{name}</FolderName>
        </IconWrapper>
    );
};

const Assets = () => {
    const [assets, setAssets] = useState<AssetModel[] | undefined>(undefined);
    const [assetType, setAssetType] = useState<AssetType>(AssetType.NONE);

    const open_directory = (dir: string) => {
        switch (dir) {
            case 'textures':
                setAssetType(AssetType.TEXTURE);
                break;

            case 'sounds':
                setAssetType(AssetType.SOUND);
                break;

            case 'scripts':
                setAssetType(AssetType.SCRIPT);
                break;

            case 'animations':
                setAssetType(AssetType.ANIMATION);
                break;

            case 'shaders':
                setAssetType(AssetType.SHADER);
                break;

            default:
                setAssetType(AssetType.NONE);
        }

        invoke('get_' + dir)
            .then((ret) => setAssets(ret as AssetModel[]))
            .catch(console.error);
    };

    return (
        <AssetsContainer>
            {(assets &&
                assets.map((asset) => (
                    <Asset
                        name={asset.name}
                        path={asset.path}
                        type={assetType}
                        key={asset.name}
                    />
                ))) ||
                directories.map((file) => (
                    <IconWrapper
                        key={file}
                        onClick={() => open_directory(file)}
                    >
                        <FaFolder size="4em" />
                        <FolderName>{file}</FolderName>
                    </IconWrapper>
                ))}
        </AssetsContainer>
    );
};
export default Assets;
