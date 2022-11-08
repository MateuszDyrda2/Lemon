import { invoke } from '@tauri-apps/api/tauri';
import { useState, useEffect, useContext } from 'react';
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
import { PanelContext, PanelContextType } from '../../../state/PanelContext';

const Scene = () => {
    const [entities, setEntities] = useState<Entity[] | undefined>(undefined);
    const { setCurrentPanel } = useContext(PanelContext) as PanelContextType;

    const [chEntity, setChosenEntity] = useRecoilState(chosenEntity);
    const entityPressed = (ent: Entity) => setChosenEntity(ent);

    const entityDoublePressed = (ent: Entity) => {
        setChosenEntity(ent);
        setCurrentPanel(PanelTypes.Components);
    };

    useEffect(() => {
        invoke('get_entities')
            .then((value) => setEntities(value as Entity[]))
            .catch((err) => console.log(err));
    }, []);

    const EntityContainer = (ent: Entity) => {
        return (
            <EntityWrapper
                selected={chEntity !== undefined && ent.id === chEntity.id}
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
            <EntityList>
                {entities && entities.map((key, _) => EntityContainer(key))}
            </EntityList>
        </SceneContainer>
    );
};

export default Scene;
