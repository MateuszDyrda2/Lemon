import { useCallback, useEffect, useRef, useState } from 'react';
import { ViewportCanvas, ViewportContainer } from './viewport.styles';
import { useResizeDetector } from 'react-resize-detector';
import {
    fragmentSource,
    initializeShaders,
    ProgramInfo,
    vertexSource,
} from './shader/shader';
import { Buffers, initializeBuffers } from './buffer/buffer';
import { appWindow } from '@tauri-apps/api/window';
import { invoke } from '@tauri-apps/api';
import { RenderingData } from '../../../props/rendering_data';
import load_textures, { Texture, TextureDefinition } from './assets/assets';
import renderScene from './renderer/renderer';

interface RenderingState {
    buffers: Buffers;
    program: ProgramInfo;
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
    const [textures, setTextures] = useState<{ [nameid: number]: Texture }>();

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

            if (!renderingState || !renderingData || !textures) return;

            const program = renderingState.program;
            const buffers = renderingState.buffers;

            renderScene(
                renderingData,
                gl,
                program,
                buffers,
                textures,
                size.width,
                size.height,
            );
        },
        [renderingState, size],
    );

    useEffect(() => {
        const canvas = canvasRef.current as HTMLCanvasElement;
        const gl = canvas.getContext('webgl');
        if (gl === null) return;

        const program = initializeShaders(gl, vertexSource, fragmentSource);
        const buffers = initializeBuffers(gl);
        if (program === null || buffers === null) return;

        setRenderingState({ program: program, buffers: buffers });

        appWindow.listen('project-opened', (_) => {
            invoke('get_rendering_data')
                .then((data) => {
                    const rd = data as RenderingData[];
                    setRenderingData(rd);

                    const arr: number[] = [];
                    for (var i = 0; i < rd.length; ++i) {
                        arr.push(rd[i].textureid);
                    }
                    invoke('get_asset_list', { ids: arr })
                        .then((ret) =>
                            setTextures(
                                load_textures(
                                    gl,
                                    ret as { [key: number]: TextureDefinition },
                                ),
                            ),
                        )
                        .catch((err) => console.log(err));
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
