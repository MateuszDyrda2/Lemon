import { AssetsContainer, IconWrapper, FolderName } from './assets.styles';
import { FaFolder } from 'react-icons/fa';
import { useEffect, useState } from 'react';
import { appWindow } from '@tauri-apps/api/window';
import { invoke } from '@tauri-apps/api';

interface AssetModel {
    name: string;
    path: string;
}

const directories = ['textures', 'sounds', 'scripts', 'animations', 'shaders'];

const Assets = () => {
    return (
        <AssetsContainer>
            {directories.map((file) => (
                <IconWrapper key={file}>
                    <FaFolder size="4em" />
                    <FolderName>{file}</FolderName>
                </IconWrapper>
            ))}
        </AssetsContainer>
    );
};
export default Assets;
