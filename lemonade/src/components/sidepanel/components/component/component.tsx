import { useCallback, useState } from 'react';
import {
    DropUp,
    DropDown,
    ComponentClass,
    ComponentHeader,
    ComponentName,
    Close,
} from './component.styles';
import RenderField from '../field/field';
import { invoke } from '@tauri-apps/api';

interface ComponentProps {
    entityid: number;
    name: string;
    fields: { [key: string]: object };
}

const Component = ({ entityid, name, fields }: ComponentProps) => {
    const [expanded, setExpanded] = useState(false);
    const onExpand = useCallback(() => setExpanded(!expanded), [expanded]);
    const onDelete = useCallback(() => {
        invoke('remove_component_from_entity', {
            entityid: entityid,
            componentname: name,
        }).catch(console.error);
    }, [entityid, name]);

    return (
        <ComponentClass>
            <ComponentHeader>
                {expanded ? (
                    <DropUp onClick={onExpand} />
                ) : (
                    <DropDown onClick={onExpand} />
                )}
                <ComponentName>{name}</ComponentName>
                <Close onClick={onDelete} />
            </ComponentHeader>
            {expanded &&
                Object.keys(fields).map((key, _) => (
                    <RenderField
                        entityid={entityid}
                        componentName={name}
                        name={key}
                        value={fields[key]}
                        key={key}
                    />
                ))}
        </ComponentClass>
    );
};

export default Component;
