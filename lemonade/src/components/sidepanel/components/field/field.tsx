import { invoke } from '@tauri-apps/api';
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
    const onChange = (e: React.ChangeEvent<HTMLInputElement>) =>
        setObj(e.target.value);

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
                        type="number"
                        value={obj as number}
                        onChange={onChange}
                        onBlur={onBlur}
                    />
                )) ||
                (typeof obj === 'string' && (
                    <ObjectInput
                        type="string"
                        value={obj as string}
                        onChange={onChange}
                        onBlur={onBlur}
                    />
                ))}
        </ObjectClass>
    );
};

const RenderField = ({ entityid, componentName, name, value }: Field) => {
    const onBlur = () => {
        console.log(`${savedValue}`);
        invoke('change_entity_component', {
            entityid: entityid,
            componentname: componentName,
            fieldname: name,
            newvalue: savedValue,
        }).catch(console.error);
    };
    const [savedValue, setSavedValue] = useState<unknown>(value);

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
