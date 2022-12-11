import { appWindow } from '@tauri-apps/api/window';
import { useEffect, useState } from 'react';
import { OutputContainer, OutputTextfield } from './output.styles';

const Output = () => {
    const [text, setText] = useState<string[]>([]);

    useEffect(() => {
        appWindow.listen('received-output', (data) =>
            setText((old) => [...old, data.payload as string]),
        );
    }, []);

    return (
        <OutputContainer>
            {text.map((value, index) => (
                <OutputTextfield key={index}>{value}</OutputTextfield>
            ))}
        </OutputContainer>
    );
};

export default Output;
