import { useCallback, useEffect, useRef, useState } from 'react';
import { ViewportCanvas, ViewportContainer } from './viewport.styles';
import { useResizeDetector } from 'react-resize-detector';
import { mat4 } from 'gl-matrix';
import {
    fragmentSource,
    initializeShaders,
    vertexSource,
} from './shader/shader';
import { initializeBuffers } from './buffer/buffer';
import { appWindow } from '@tauri-apps/api/window';
import { invoke } from '@tauri-apps/api';
import { RenderingData } from '../../../props/rendering_data';

interface RenderingState {
    buffer: WebGLBuffer;
    program: WebGLProgram;
}
interface Size {
    width: number;
    height: number;
}

const Viewport = () => {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const [renderingState, setRenderingState] = useState<RenderingState>();
    const [size, setSize] = useState<Size>({ width: 300, height: 300 });
    const [renderingData, setRenderingData] = useState<RenderingData[]>();

    const onResize = useCallback(
        (width: number | undefined, height: number | undefined) => {
            if (width && height) {
                setSize({ width: width, height: height });
            }
        },
        [setSize],
    );
    const { ref } = useResizeDetector({ onResize: onResize });

    const drawScene = useCallback(
        (gl: WebGLRenderingContext) => {
            if (!size) return;

            gl.clearColor(0.0, 0.0, 0.0, 1.0);
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.viewport(0, 0, size.width, size.height);

            if (!renderingState) return;
            const program = renderingState.program;
            const buffer = renderingState.buffer;

            const vertexPos = gl.getAttribLocation(program, 'a_position');
            const projectionPos = gl.getUniformLocation(program, 'projection');
            const modelPos = gl.getUniformLocation(program, 'model');
            if (
                vertexPos === null ||
                projectionPos === null ||
                modelPos === null
            )
                return;

            const projection = mat4.create();
            const hWidth = size.width * 0.5;
            const hHeight = size.height * 0.5;
            mat4.ortho(projection, -hWidth, hWidth, -hHeight, hHeight, -1, 1);
            const model = mat4.create();

            gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
            gl.vertexAttribPointer(vertexPos, 2, gl.FLOAT, false, 0, 0);
            gl.enableVertexAttribArray(vertexPos);
            gl.useProgram(program);
            gl.uniformMatrix4fv(projectionPos, false, projection);
            gl.uniformMatrix4fv(modelPos, false, model);
            gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        },
        [renderingState, size],
    );

    useEffect(() => {
        const canvas = canvasRef.current as HTMLCanvasElement;
        const gl = canvas.getContext('webgl');
        if (gl === null) return;

        const program = initializeShaders(gl, vertexSource, fragmentSource);
        const buffer = initializeBuffers(gl);
        if (program === null || buffer === null) return;

        setRenderingState({ program: program, buffer: buffer });

        appWindow.listen('project-opened', (_) => {
            invoke('get_rendering_data')
                .then((data) => {
                    setRenderingData(data as RenderingData[]);
                })
                .catch((err) => console.log(err));
        });
    }, []);

    useEffect(() => {
        if (canvasRef.current === null) return;

        const canvas = canvasRef.current as HTMLCanvasElement;
        const gl = canvas.getContext('webgl');
        if (gl === null) return;

        var animationFrameId: number;
        const render = () => {
            drawScene(gl);
            animationFrameId = window.requestAnimationFrame(render);
        };
        render();
        return () => window.cancelAnimationFrame(animationFrameId);
    }, [drawScene]);

    return (
        <ViewportContainer ref={ref}>
            <ViewportCanvas
                ref={canvasRef}
                width={size.width}
                height={size.height}
            />
        </ViewportContainer>
    );
};

export default Viewport;
