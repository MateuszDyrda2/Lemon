import { invoke } from '@tauri-apps/api/tauri';
import { useState, useEffect } from 'react';
import Container from '../../container/container';
import { PanelTypes } from '../../../props/panel-types';
import {
    SceneContainer,
    EntityName,
    EntityWrapper,
    EntityList,
} from './scene.styles';
import { useRecoilState } from 'recoil';
import { Entity, chosenEntity } from '../../../state/chosen_entity';

interface Props {
    setTab: React.Dispatch<any>;
}

const Scene = ({ setTab }: Props) => {
    const [entities, setEntities] = useState<Entity[] | undefined>(undefined);

    const [chEntity, setChosenEntity] = useRecoilState(chosenEntity);
    const entityPressed = (ent: Entity) => setChosenEntity(ent);

    const entityDoublePressed = (ent: Entity) => setTab(PanelTypes.Components);
    useEffect(() => {
        invoke('get_entities')
            .then((value) => setEntities(value as Entity[]))
            .catch((err) => console.log(err));
    }, []);

    const EntityContainer = (ent: Entity) => {
        return (
            <EntityWrapper
                selected={chEntity && ent.id === chEntity.id}
                key={ent.id}
                onClick={() => entityPressed(ent)}
                onDoubleClick={() => entityDoublePressed(ent)}
            >
                <EntityName>{ent.name}</EntityName>
            </EntityWrapper>
        );
    };

    return (
        <SceneContainer>
            <Container header="Scene Hierarchy">
                <EntityList>
                    {entities && entities.map((key, _) => EntityContainer(key))}
                </EntityList>
            </Container>
        </SceneContainer>
    );
};

export default Scene;
