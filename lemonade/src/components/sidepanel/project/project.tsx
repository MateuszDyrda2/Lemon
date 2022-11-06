import { open } from '@tauri-apps/api/dialog';
import { exit } from '@tauri-apps/api/process';

import { useEffect, useState } from 'react';
import LemonIcon from '../../../img/lemon.svg';
import {
    ProjectContainer,
    LemonImg,
    ButtonGroup,
    Button,
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
        // invoke open project
    };

    useEffect(() => {
        // get project name
    }, []);
    return (
        <ProjectContainer>
            <LemonImg src={LemonIcon} alt="Lemon logo" width={150} />
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
