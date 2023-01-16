import { invoke } from '@tauri-apps/api/tauri';
import { useCallback, useEffect, useState } from 'react';
import {
    SettingName,
    SettingsContainer,
    SettingsOption,
    SettingValue,
    Submit,
} from './settings.styles';

interface ProjectSettings {
    project_name: string;
    assets_path: string;
    scene_path: string;
    src_path: string;
    types_path: string;
    exec_path: string;
    scenes: string[];
}

const Settings = () => {
    const [projectSettings, setProjectSettings] = useState<
        ProjectSettings | undefined
    >();

    const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        const { name, value } = e.target;
        setProjectSettings(
            (prevState) =>
                ({
                    ...prevState,
                    [name]: value,
                } as ProjectSettings),
        );
    };

    useEffect(() => {
        invoke('get_settings')
            .then((ret) => {
                setProjectSettings(ret as ProjectSettings);
            })
            .catch(console.error);
    }, []);

    const saveSettings = () => {
        if (!projectSettings) return;
        invoke('set_settings', { projectSettings: projectSettings }).catch(
            console.error,
        );
    };

    return (
        <SettingsContainer>
            {projectSettings && (
                <>
                    <SettingsOption>
                        <SettingName>Project Name:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.project_name}
                            name="project_name"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <SettingsOption>
                        <SettingName>Assets Path:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.assets_path}
                            name="assets_path"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <SettingsOption>
                        <SettingName>Scene Path:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.scene_path}
                            name="scene_path"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <SettingsOption>
                        <SettingName>Source Path:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.src_path}
                            name="src_path"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <SettingsOption>
                        <SettingName>Types Path:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.types_path}
                            name="types_path"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <SettingsOption>
                        <SettingName>Executable Path:</SettingName>
                        <SettingValue
                            type="text"
                            value={projectSettings.exec_path}
                            name="exec_path"
                            onChange={handleChange}
                        />
                    </SettingsOption>
                    <Submit onClick={saveSettings}>Save</Submit>
                </>
            )}
        </SettingsContainer>
    );
};

export default Settings;
