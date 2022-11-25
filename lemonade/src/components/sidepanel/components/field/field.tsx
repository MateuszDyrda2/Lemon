import { invoke } from '@tauri-apps/api';
import { appWindow } from '@tauri-apps/api/window';
import { useState } from 'react';
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

const RenderObject = ({ onBlur, obj, setObj }: ObjectProps) => {
    return (
        <ObjectClass>
            {(Object.prototype.toString.call(obj) === '[object Array]' &&
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
                (typeof obj === 'object' &&
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
                (typeof obj === 'number' && (
                    <ObjectInput
                        type="text"
                        value={obj as number}
                        onChange={(e) => {
                            setObj(
                                parseFloat(
                                    e.target.value.replace(/[^0-9.-]/g, ''),
                                ),
                            );
                        }}
                        onBlur={onBlur}
                    />
                )) ||
                (typeof obj === 'string' && (
                    <ObjectInput
                        type="text"
                        value={obj as string}
                        onChange={(e) => setObj(e.target.value)}
                        onBlur={onBlur}
                    />
                ))}
        </ObjectClass>
    );
};

const RenderField = ({ entityid, componentName, name, value }: Field) => {
    const onBlur = () => {
        invoke('change_entity_component', {
            entityid: entityid,
            componentname: componentName,
            fieldname: name,
            newvalue: savedValue,
        }).catch(console.error);
    };
    const [savedValue, setSavedValue] = useState<unknown>(value);
    console.log('rerender');

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
