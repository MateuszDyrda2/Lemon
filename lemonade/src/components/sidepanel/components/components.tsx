import { invoke } from '@tauri-apps/api/tauri';
import { useState, useEffect } from 'react';
import { useRecoilState } from 'recoil';
import { chosenEntity } from '../../../state/chosen_entity';
import { RiAddFill } from 'react-icons/ri';

import {
    ComponentsContainer,
    IconWrapper,
    DropdownWrapper,
    EntityNameContainer,
    EntityName,
} from './components.styles';
import Dropdown from './dropdown/dropdown';

import Component from './component/component';

interface EntityComponents {
    components: { [name: string]: { [key: string]: object } };
    name: string;
}

const Components = () => {
    const [chEntity, setChEntity] = useRecoilState(chosenEntity);
    const [components, setComponents] = useState<EntityComponents | undefined>(
        undefined,
    );
    const [dropdown, setDropdown] = useState(false);
    const [componentDefs, setComponentDefs] = useState<string[]>([]);

    useEffect(() => {
        if (chEntity) {
            invoke('get_components_for_entity', {
                entityid: chEntity.id,
            })
                .then((value) => setComponents(value as EntityComponents))
                .catch(console.error);
        }
        invoke('get_components')
            .then((value) => setComponentDefs(value as string[]))
            .catch(console.error);
    }, [chEntity]);

    const chooseComponent = (ch: string) => {
        setDropdown(false);
        invoke('add_component_to_entity', {
            entityid: chEntity?.id,
            componentname: ch,
        })
            .then((value) => setComponents(value as EntityComponents))
            .catch(console.error);
    };

    const changeName = (e: React.ChangeEvent<HTMLInputElement>) => {
        setComponents({ ...components!, name: e.target.value });
    };
    const acceptNameChange = (e: React.FocusEvent<HTMLInputElement>) => {
        invoke('set_entity_name', {
            entityid: chEntity?.id,
            name: e.target.value,
        })
            .then((ret) => setChEntity({ ...chEntity!, name: ret as string }))
            .catch(console.error);
    };

    return (
        <ComponentsContainer>
            {components && (
                <>
                    <EntityNameContainer>
                        <EntityName
                            value={components && components.name}
                            onBlur={acceptNameChange}
                            onChange={changeName}
                        />
                    </EntityNameContainer>
                    {Object.keys(components.components).map((key, _) => (
                        <Component
                            entityid={chEntity?.id ?? 0}
                            name={key}
                            fields={components.components[key]}
                            key={key}
                        />
                    ))}

                    <DropdownWrapper>
                        <IconWrapper onClick={() => setDropdown((old) => !old)}>
                            <RiAddFill size="2em" />
                        </IconWrapper>
                        <Dropdown
                            isOpen={dropdown}
                            options={componentDefs}
                            onSelect={chooseComponent}
                        />
                    </DropdownWrapper>
                </>
            )}
        </ComponentsContainer>
    );
};

export default Components;
