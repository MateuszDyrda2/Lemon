import { open } from '@tauri-apps/api/dialog';
import { exit } from '@tauri-apps/api/process';
import { invoke } from '@tauri-apps/api/tauri';

import { useState } from 'react';
import LemonIcon from '../../../img/lemon.svg';
import {
    ProjectContainer,
    LemonImg,
    ButtonGroup,
    Button,
    ProjectName,
} from './project.styles';

const Project = () => {
    const [project, setProject] = useState<string | undefined>(undefined);

    const openProject = async () => {
        const selected = await open({
            multiple: false,
            filters: [
                {
                    name: 'Lemon project',
                    extensions: ['lmnproj'],
                },
            ],
        });

        if (selected) {
            invoke('open_project', { path: selected })
                .then((name) => setProject(name as string))
                .catch((error) => console.log(error));
        }
    };

    return (
        <ProjectContainer>
            <LemonImg src={LemonIcon} alt="Lemon logo" width={150} />
            <ProjectName>{project !== undefined && project}</ProjectName>
            <ButtonGroup>
                <Button>New project</Button>
                <Button onClick={() => openProject()}>Open Project</Button>
                <Button>Save</Button>
                <Button>Save as</Button>
                <Button onClick={() => exit(0)}>Exit</Button>
            </ButtonGroup>
        </ProjectContainer>
    );
};

export default Project;
