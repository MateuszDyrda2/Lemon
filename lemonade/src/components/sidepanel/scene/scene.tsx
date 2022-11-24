import { invoke } from '@tauri-apps/api/tauri';
import { useState, useEffect, useContext } from 'react';
import { PanelTypes } from '../../../props/panel-types';
import {
    SceneContainer,
    EntityName,
    EntityWrapper,
    EntityList,
    AddWrapper,
} from './scene.styles';
import { useRecoilState } from 'recoil';
import { Entity, chosenEntity } from '../../../state/chosen_entity';
import { PanelContext, PanelContextType } from '../../../state/PanelContext';
import { RiAddFill } from 'react-icons/ri';

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

    const addEntity = () => {
        invoke('add_entity')
            .then((value) => setEntities(value as Entity[]))
            .catch(console.error);
    };

    return (
        <SceneContainer>
            {entities && (
                <>
                    <EntityList>
                        {entities.map((key, _) => EntityContainer(key))}
                    </EntityList>
                    <AddWrapper onClick={() => addEntity()}>
                        <RiAddFill size="2em" />
                    </AddWrapper>
                </>
            )}
        </SceneContainer>
    );
};

export default Scene;
