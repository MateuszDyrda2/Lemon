import { invoke } from '@tauri-apps/api/tauri';
import { useCallback, useState, useEffect } from 'react';
import { useRecoilState } from 'recoil';
import { chosenEntity } from '../../../state/chosen_entity';
import { RiAddFill } from 'react-icons/ri';

import {
    ComponentsContainer,
    ObjectClass,
    FieldClass,
    FieldValues,
    ComponentClass,
    ComponentHeader,
    FieldName,
    ComponentName,
    DropUp,
    ObjectInput,
    DropDown,
    IconWrapper,
    DropdownWrapper,
} from './components.styles';
import Dropdown from './dropdown/dropdown';

interface Field {
    name: string;
    value: object;
}

interface ComponentProps {
    name: string;
    fields: { [key: string]: object };
}

interface ObjectProps {
    obj: object;
}

const RenderObject = ({ obj }: ObjectProps) => {
    const [val, setVal] = useState<unknown>(obj);
    return (
        <ObjectClass>
            {(Object.prototype.toString.call(obj) === '[object Array]' &&
                (val as object[]).map((key, index) => (
                    <RenderObject obj={key} key={index} />
                ))) ||
                (typeof obj === 'object' &&
                    Object.keys(val as { [key: string]: object }).map(
                        (k, index) => (
                            <div>
                                <p>{k}</p>
                                <RenderObject
                                    obj={val as { [key: string]: object }}
                                    key={index}
                                />
                            </div>
                        ),
                    )) ||
                (typeof obj === 'number' && (
                    <ObjectInput
                        type="number"
                        value={val as number}
                        onChange={(e) => setVal(e.target.value)}
                    />
                )) ||
                (typeof obj === 'string' && (
                    <ObjectInput
                        type="string"
                        value={val as string}
                        onChange={(e) => setVal(e.target.value)}
                    />
                ))}
        </ObjectClass>
    );
};

const RenderField = ({ name, value }: Field) => {
    return (
        <FieldClass>
            <FieldName>{name}</FieldName>
            <FieldValues>
                <RenderObject obj={value as object} />
            </FieldValues>
        </FieldClass>
    );
};

const Component = ({ name, fields }: ComponentProps) => {
    const [expanded, setExpanded] = useState(false);
    const onExpand = useCallback(() => setExpanded(!expanded), [expanded]);

    return (
        <ComponentClass>
            <ComponentHeader onClick={onExpand}>
                {expanded ? <DropUp /> : <DropDown />}
                <ComponentName>{name}</ComponentName>
            </ComponentHeader>
            {expanded &&
                Object.keys(fields).map((key, _) => (
                    <RenderField name={key} value={fields[key]} key={key} />
                ))}
        </ComponentClass>
    );
};

const Components = () => {
    const [chEntity] = useRecoilState(chosenEntity);
    const [components, setComponents] = useState<
        { [name: string]: { [key: string]: object } } | undefined
    >(undefined);
    const [dropdown, setDropdown] = useState(false);
    const [componentDefs, setComponentDefs] = useState<string[]>([]);

    const fetchComponentsForEntity = async () => {
        if (chEntity) {
            const value = await invoke('get_components_for_entity', {
                entityid: chEntity.id,
            });
            setComponents(
                value as { [name: string]: { [key: string]: object } },
            );
        }
    };

    const fetchComponents = async () => {
        const value = await invoke('get_components');
        setComponentDefs(value as string[]);
    };

    useEffect(() => {
        fetchComponentsForEntity().catch(console.error);
        fetchComponents().catch(console.error);
    }, [chEntity]);

    const chooseComponent = async (ch: DropdownOption) => {
        setDropdown(false);
        try {
            await invoke('add_component_to_entity', {
                entityid: chEntity?.id,
                componentname: ch,
            });

            await fetchComponentsForEntity();
        } catch (err) {
            console.error(err);
        }
    };

    return (
        <ComponentsContainer>
            {components &&
                Object.keys(components).map((key, _) => (
                    <Component name={key} fields={components[key]} key={key} />
                ))}

            <DropdownWrapper isVisible={!!components}>
                <IconWrapper onClick={() => setDropdown((old) => !old)}>
                    <RiAddFill size="2em" />
                </IconWrapper>
                <Dropdown
                    isOpen={dropdown}
                    options={componentDefs}
                    onSelect={chooseComponent}
                />
            </DropdownWrapper>
        </ComponentsContainer>
    );
};

export default Components;
