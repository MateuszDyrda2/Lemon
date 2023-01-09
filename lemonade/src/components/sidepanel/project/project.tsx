import { open, save } from '@tauri-apps/api/dialog';
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

    const openProject = () => {
        open({
            multiple: false,
            filters: [
                {
                    name: 'Lemon project',
                    extensions: ['lmnproj'],
                },
            ],
        })
            .then((selected) => {
                if (selected) {
                    invoke('open_project', { path: selected })
                        .then((name) =>
                            setProjectState({ name: name as string }),
                        )
                        .catch(console.error);
                }
            })
            .catch(console.error);
    };

    const saveProject = () => {
        invoke('save').catch(console.error);
    };

    const createProject = () => {
        save({
            filters: [
                {
                    name: 'NewProject',
                    extensions: [],
                },
            ],
        }).then((path) => {
            invoke('create_project', { path: path }).catch(console.error);
        });
    };

    return (
        <ProjectContainer>
            <LemonImg src={LemonIcon} alt="Lemon logo" width={150} />
            <ProjectName>
                {projectState !== undefined && projectState.name}
            </ProjectName>
            <ButtonGroup>
                <Button onClick={createProject}>New project</Button>
                <Button onClick={openProject}>Open Project</Button>
                <Button onClick={saveProject}>Save</Button>
                <Button>Save as</Button>
                <Button onClick={() => exit(0)}>Exit</Button>
            </ButtonGroup>
        </ProjectContainer>
    );
};

export default Project;
