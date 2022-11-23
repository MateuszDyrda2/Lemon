import { AssetsContainer, IconWrapper, FolderName } from './assets.styles';
import { FaFolder } from 'react-icons/fa';
import { AiFillFile } from 'react-icons/ai';
import { useState } from 'react';
import { invoke } from '@tauri-apps/api';

const directories = ['textures', 'sounds', 'scripts', 'animations', 'shaders'];

interface AssetModel {
    name: string;
    path: string;
}

const Assets = () => {
    const [assets, setAssets] = useState<AssetModel[] | undefined>(undefined);

    const open_directory = (dir: string) => {
        invoke('get_' + dir)
            .then((ret) => setAssets(ret as AssetModel[]))
            .catch(console.error);
    };
    return (
        <AssetsContainer>
            {(assets &&
                assets.map((asset) => (
                    <IconWrapper>
                        <AiFillFile size="4em" />
                        <FolderName>{asset.name}</FolderName>
                    </IconWrapper>
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
