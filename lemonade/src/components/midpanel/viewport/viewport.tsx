import { useCallback, useEffect, useReducer, useRef, useState } from 'react';
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
import {
    Camera,
    createCamera,
    moveCameraBy,
    resetCamera,
    zoomCamera,
} from './camera/camera';

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
    const [update, forceUpdate] = useReducer((x) => x + 1, 0);
    const [mouseDown, setMouseDown] = useState(false);
    const [mousePos, setMousePos] = useState<{ x: number; y: number }>({
        x: 0,
        y: 0,
    });
    const [isMouseOver, setIsMouseOver] = useState(false);
    const [camera, setCamera] = useState<Camera | undefined>(undefined);

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

            const { program, buffers } = renderingState;

            renderScene(
                renderingData,
                gl,
                program,
                buffers,
                camera as Camera,
                textures,
                size.width,
                size.height,
            );
        },
        [renderingState, size, update, camera],
    );

    useEffect(() => {
        const canvas = canvasRef.current as HTMLCanvasElement;
        const gl = canvas.getContext('webgl');
        if (gl === null) return;

        const program = initializeShaders(gl, vertexSource, fragmentSource);
        const buffers = initializeBuffers(gl);
        setCamera(createCamera(1.0));
        if (program === null || buffers === null) return;

        setRenderingState({
            program: program,
            buffers: buffers,
        });

        appWindow.listen('project-opened', async (_) => {
            try {
                const data = await invoke('get_rendering_data');
                const rd = data as RenderingData[];
                setRenderingData(rd);

                const arr: number[] = [];
                for (var i = 0; i < rd.length; ++i) {
                    arr.push(rd[i].textureid);
                }
                const ret = await invoke('get_asset_list', { ids: arr });
                const texs = await load_textures(
                    gl,
                    ret as { [key: number]: TextureDefinition },
                );
                setTextures(texs);
                setCamera(resetCamera());
                forceUpdate();
            } catch (e) {
                console.log(e);
            }
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
    }, [drawScene, update]);

    useEffect(() => {
        if (!mouseDown) return;

        const onMouseMove = (e: MouseEvent) => {
            if (mouseDown) {
                const offset = [
                    mousePos.x - e.clientX,
                    -1 * (mousePos.y - e.clientY),
                ];
                setCamera((old) => moveCameraBy(old as Camera, offset));
                setMousePos({ x: e.pageX, y: e.pageY });
            }
        };

        const onMouseUp = () => {
            setMouseDown(false);
        };
        window.addEventListener('mousemove', onMouseMove);
        window.addEventListener('mouseup', onMouseUp);
        return () => {
            window.removeEventListener('mousemove', onMouseMove);
            window.removeEventListener('mouseup', onMouseUp);
        };
    }, [mouseDown, mousePos]);

    const handleNavigation = (e: React.WheelEvent) => {
        const c = camera as Camera;
        if (e.deltaY > 0) {
            setCamera(zoomCamera(c, -0.1));
        } else if (e.deltaY < 0) {
            setCamera(zoomCamera(c, 0.1));
        }
    };

    const onMouseDown = (e: React.MouseEvent) => {
        setMousePos({ x: e.pageX, y: e.pageY });
        setMouseDown(true);
    };

    return (
        <ViewportContainer ref={ref}>
            <ViewportCanvas
                onMouseDown={onMouseDown}
                onMouseEnter={() => setIsMouseOver(true)}
                onMouseLeave={() => setIsMouseOver(false)}
                onWheel={handleNavigation}
                ref={canvasRef}
                width={size.width}
                height={size.height}
            />
        </ViewportContainer>
    );
};

export default Viewport;
