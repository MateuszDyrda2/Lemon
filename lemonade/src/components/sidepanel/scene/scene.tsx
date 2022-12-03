import { invoke } from '@tauri-apps/api/tauri';
import { useState, useEffect, useContext, useCallback } from 'react';
import { PanelTypes } from '../../../props/panel-types';
import {
    SceneContainer,
    EntityName,
    EntityWrapper,
    EntityList,
    AddWrapper,
    Close,
} from './scene.styles';
import { useRecoilState } from 'recoil';
import { Entity, chosenEntity } from '../../../state/chosen_entity';
import { PanelContext, PanelContextType } from '../../../state/PanelContext';
import { RiAddFill } from 'react-icons/ri';

interface EntityModel {
    id: number;
    name: string;
}

interface EntityContainerProps {
    chEntity: Entity | undefined;
    setChosenEntity: { (ent: Entity): void };
    setCurrentPanel: { (panel: PanelTypes): void };
    ent: EntityModel;
    removeEntity: { (entityid: number): void };
}

const EntityContainer = ({
    chEntity,
    setChosenEntity,
    setCurrentPanel,
    ent,
    removeEntity,
}: EntityContainerProps) => {
    const entityPressed = (ent: Entity) => setChosenEntity(ent);

    const entityDoublePressed = (ent: Entity) => {
        setChosenEntity(ent);
        setCurrentPanel(PanelTypes.Components);
    };

    return (
        <EntityWrapper
            selected={chEntity !== undefined && ent.id === chEntity.id}
            key={ent.id}
            onClick={() => entityPressed(ent)}
            onDoubleClick={() => entityDoublePressed(ent)}
        >
            <EntityName>{ent.name}</EntityName>
            <Close onClick={() => removeEntity(ent.id)} />
        </EntityWrapper>
    );
};

const Scene = () => {
    const [entities, setEntities] = useState<EntityModel[] | undefined>(
        undefined,
    );
    const { setCurrentPanel } = useContext(PanelContext) as PanelContextType;

    const [chEntity, setChosenEntity] = useRecoilState(chosenEntity);

    useEffect(() => {
        invoke('get_entities')
            .then((value) => setEntities(value as EntityModel[]))
            .catch((err) => console.log(err));
    }, []);

    const addEntity = useCallback(() => {
        invoke('add_entity')
            .then((value) => setEntities(value as EntityModel[]))
            .catch(console.error);
    }, [entities]);

    const removeEntity = useCallback(
        (entityid: number) => {
            invoke('remove_entity', { entityid: entityid })
                .then((value) => setEntities(value as EntityModel[]))
                .catch(console.error);
        },
        [entities],
    );

    return (
        <SceneContainer>
            {entities && (
                <>
                    <EntityList>
                        {entities.map((key, _) => (
                            <EntityContainer
                                chEntity={chEntity}
                                setChosenEntity={setChosenEntity}
                                setCurrentPanel={setCurrentPanel}
                                ent={key}
                                removeEntity={removeEntity}
                            />
                        ))}
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
