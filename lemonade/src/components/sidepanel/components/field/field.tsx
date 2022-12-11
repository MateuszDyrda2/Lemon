import { invoke } from '@tauri-apps/api';
import { useCallback, useEffect, useState } from 'react';
import { useDrop } from 'react-dnd';
import {
    DraggableAsset,
    DraggableType,
} from '../../../../state/draggable_type';
import {
    ObjectClass,
    ObjectInput,
    FieldName,
    FieldClass,
    FieldValues,
} from './field.styles';

interface Field {
    entityid: number;
    componentName: string;
    name: string;
    value: object;
}

interface ObjectProps {
    onBlur: { (): void };
    obj: unknown;
    setObj: { (newObject: unknown): void };
}

const TextInput = ({ onBlur, obj, setObj }: ObjectProps) => {
    const update = useCallback(
        (newValue: string) => {
            setObj(newValue);
        },
        [obj],
    );

    const [, drop] = useDrop(
        () => ({
            accept: DraggableType.ASSET,
            drop: (item: DraggableAsset) => {
                console.log(`${item.name} ${item.path} ${item.assetType}`);
                update(item.name);
                return undefined;
            },
        }),
        [update],
    );

    return (
        <ObjectInput
            ref={drop}
            type="text"
            value={obj as string}
            onChange={(e) => update(e.target.value)}
            onBlur={onBlur}
        />
    );
};

const NumberInput = ({ onBlur, obj, setObj }: ObjectProps) => {
    const update = useCallback(
        (value: string) => {
            if (value.length == 0) {
                setObj(0);
                return;
            }
            const parse = parseFloat(value);

            if (!Object.is(NaN, parse)) {
                setObj(parse);
            }
        },
        [obj],
    );

    return (
        <ObjectInput
            type="number"
            value={obj as number}
            onChange={(e) => update(e.target.value)}
            onBlur={onBlur}
            step="any"
        />
    );
};

const BoolInput = ({ onBlur, obj, setObj }: ObjectProps) => {
    const update = useCallback(() => setObj((old: boolean) => !old), [obj]);

    return (
        <ObjectInput
            type="checkbox"
            checked={obj as boolean}
            onChange={update}
            onBlur={onBlur}
        />
    );
};

const RenderObject = ({ onBlur, obj, setObj }: ObjectProps) => {
    return (
        <ObjectClass>
            {(Object.prototype.toString.call(obj) == '[object Array]' &&
                (obj as object[]).map((key, index) => (
                    <RenderObject
                        onBlur={onBlur}
                        obj={key}
                        key={index}
                        setObj={(newObject: unknown) =>
                            setObj((old: unknown[]) => {
                                const newArray = [...old];
                                newArray[index] = newObject;
                                return newArray;
                            })
                        }
                    />
                ))) ||
                (typeof obj == 'object' &&
                    Object.keys(obj as { [key: string]: object }).map(
                        (k, index) => (
                            <div>
                                <p>{k}</p>
                                <RenderObject
                                    onBlur={onBlur}
                                    obj={obj as { [key: string]: object }}
                                    key={index}
                                    setObj={(newObject: unknown) =>
                                        setObj({ ...obj, [index]: newObject })
                                    }
                                />
                            </div>
                        ),
                    )) ||
                (typeof obj == 'number' && (
                    <NumberInput obj={obj} setObj={setObj} onBlur={onBlur} />
                )) ||
                (typeof obj == 'string' && (
                    <TextInput obj={obj} setObj={setObj} onBlur={onBlur} />
                )) ||
                (typeof obj === 'boolean' && (
                    <BoolInput obj={obj} setObj={setObj} onBlur={onBlur} />
                ))}
        </ObjectClass>
    );
};

const RenderField = ({ entityid, componentName, name, value }: Field) => {
    const [savedValue, setSavedValue] = useState<unknown>();

    useEffect(() => setSavedValue(value), [value]);

    const onBlur = useCallback(() => {
        invoke('change_entity_component', {
            entityid: entityid,
            componentname: componentName,
            fieldname: name,
            newvalue: savedValue,
        }).catch(console.error);
    }, [savedValue]);

    return (
        <FieldClass>
            <FieldName>{name}</FieldName>
            <FieldValues>
                <RenderObject
                    onBlur={onBlur}
                    obj={savedValue as object}
                    setObj={setSavedValue}
                />
            </FieldValues>
        </FieldClass>
    );
};

export default RenderField;
