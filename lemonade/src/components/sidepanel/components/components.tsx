import { invoke } from '@tauri-apps/api/tauri';
import { useCallback, useState, useEffect } from 'react';
import { useRecoilState } from 'recoil';
import { chosenEntity } from '../../../state/chosen_entity';

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
} from './components.styles';

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

    useEffect(() => {
        if (chEntity) {
            invoke('get_components_for_entity', { entityid: chEntity.id })
                .then((value) =>
                    setComponents(
                        value as { [name: string]: { [key: string]: object } },
                    ),
                )
                .catch((err) => console.log(err));
        }
    }, [chEntity]);

    return (
        <ComponentsContainer>
            {components &&
                Object.keys(components).map((key, _) => (
                    <Component name={key} fields={components[key]} key={key} />
                ))}
        </ComponentsContainer>
    );
};

export default Components;
