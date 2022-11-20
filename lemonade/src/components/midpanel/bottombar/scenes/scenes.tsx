import { invoke } from '@tauri-apps/api';
import { useEffect, useState } from 'react';
import { ScenesContainer, IconWrapper, SceneName } from './scenes.styles';
import { BsFillFileEarmarkFill } from 'react-icons/bs';
import { appWindow } from '@tauri-apps/api/window';

const Scenes = () => {
    const [scenes, setScenes] = useState<string[]>([]);
    useEffect(() => {
        appWindow.listen('project-opened', (_) => {
            invoke('get_scenes')
                .then((res) => setScenes(res as string[]))
                .catch(console.error);
        });

        invoke('get_scenes')
            .then((res) => setScenes(res as string[]))
            .catch(console.error);
    }, []);

    return (
        <ScenesContainer>
            {scenes.map((scene) => (
                <IconWrapper>
                    <BsFillFileEarmarkFill size="4em" />
                    <SceneName>{scene}</SceneName>
                </IconWrapper>
            ))}
        </ScenesContainer>
    );
};
export default Scenes;
