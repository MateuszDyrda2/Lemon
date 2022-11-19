import { useCallback, useState } from 'react';
import {
    DropUp,
    DropDown,
    ComponentClass,
    ComponentHeader,
    ComponentName,
} from './component.styles';
import RenderField from '../field/field';

interface ComponentProps {
    entityid: number;
    name: string;
    fields: { [key: string]: object };
}

const Component = ({ entityid, name, fields }: ComponentProps) => {
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
