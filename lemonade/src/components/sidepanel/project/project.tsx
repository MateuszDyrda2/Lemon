import { open } from '@tauri-apps/api/dialog';
import { exit } from '@tauri-apps/api/process';
import { invoke } from '@tauri-apps/api/tauri';

import { useRecoilState } from 'recoil';
import LemonIcon from '../../../img/lemon.svg';
import { current_project } from '../../../state/current_project';
import {
    ProjectContainer,
    LemonImg,
    ButtonGroup,
    Button,
    ProjectName,
} from './project.styles';

const Project = () => {
    const [projectState, setProjectState] = useRecoilState(current_project);

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
                .then((name) => setProjectState({ name: name as string }))
                .catch((error) => console.log(error));
        }
    };

    return (
        <ProjectContainer>
            <LemonImg src={LemonIcon} alt="Lemon logo" width={150} />
            <ProjectName>
                {projectState !== undefined && projectState.name}
            </ProjectName>
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
