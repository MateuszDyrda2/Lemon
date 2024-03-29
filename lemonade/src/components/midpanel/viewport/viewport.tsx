import { useCallback, useEffect, useReducer, useRef, useState } from 'react';
import { ViewportCanvas, ViewportContainer } from './viewport.styles';
import { useResizeDetector } from 'react-resize-detector';
import { initializeShaders, ProgramInfo } from './shader/shader';
import { Buffers, initializeBuffers } from './buffer/buffer';
import { appWindow } from '@tauri-apps/api/window';
import { invoke } from '@tauri-apps/api';
import { DebugData, RenderingData } from '../../../props/rendering_data';
import load_textures, { Texture, TextureDefinition } from './assets/assets';
import renderScene, { testMouse } from './renderer/renderer';
import {
    Camera,
    createCamera,
    moveCameraBy,
    resetCamera,
    zoomCamera,
} from './camera/camera';
import { useRecoilState } from 'recoil';
import { chosenEntity } from '../../../state/chosen_entity';
import { selectedTools } from '../../../state/selected_tools';

interface RenderingState {
    buffers: Buffers;
    program: ProgramInfo;
    gl: WebGLRenderingContext;
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
    const [debugData, setDebugData] = useState<DebugData[]>();
    const [textures, setTextures] = useState<{ [nameid: number]: Texture }>();
    const [update, forceUpdate] = useReducer((x) => x + 1, 0);
    const [mouseDown, setMouseDown] = useState(false);
    const [mousePos, setMousePos] = useState<{ x: number; y: number }>({
        x: 0,
        y: 0,
    });
    const [camera, setCamera] = useState<Camera | undefined>(undefined);
    const [chEntity, setChEntity] = useRecoilState(chosenEntity);
    const [tools] = useRecoilState(selectedTools);

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
            gl.clear(
                gl.COLOR_BUFFER_BIT |
                    gl.DEPTH_BUFFER_BIT |
                    gl.STENCIL_BUFFER_BIT,
            );
            gl.viewport(0, 0, size.width, size.height);

            if (!renderingState || !renderingData || !textures || !debugData)
                return;

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
                chEntity,
                tools.grid,
                debugData,
            );
        },
        [renderingState, size, update, camera, chEntity],
    );

    const reloadScene = useCallback((gl: WebGLRenderingContext) => {
        invoke('get_rendering_data')
            .then((data) => {
                const rd = data as RenderingData[];
                setRenderingData(rd);

                const arr: number[] = [];
                for (var i = 0; i < rd.length; ++i) arr.push(rd[i].textureid);

                invoke('get_asset_list', { ids: arr })
                    .then((ret) => {
                        load_textures(
                            gl,
                            ret as { [key: number]: TextureDefinition },
                        )
                            .then((texs) => {
                                setTextures(texs);
                                setCamera(resetCamera());

                                invoke('get_debug_data')
                                    .then((data) =>
                                        setDebugData(data as DebugData[]),
                                    )
                                    .catch(console.error);
                                forceUpdate();
                            })
                            .catch(console.error);
                    })
                    .catch(console.error);
            })
            .catch(console.error);
    }, []);

    useEffect(() => {
        const canvas = canvasRef.current as HTMLCanvasElement;
        const gl = canvas.getContext('webgl', { stencil: true });
        if (gl === null) return;

        gl.enable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

        gl.enable(gl.DEPTH_TEST);
        gl.enable(gl.STENCIL_TEST);
        gl.stencilOp(gl.KEEP, gl.KEEP, gl.REPLACE);
        gl.stencilMask(0x00);
        const program = initializeShaders(gl);
        const buffers = initializeBuffers(gl);
        setCamera(createCamera(1.0));
        if (program === null || buffers === null) return;

        setRenderingState({
            program: program,
            buffers: buffers,
            gl: gl,
        });

        const unlisten1 = appWindow.listen('project-opened', (_) =>
            reloadScene(gl),
        );
        const unlisten2 = appWindow.listen('scene-changed', (_) => {
            reloadScene(gl);
        });

        return () => {
            unlisten1.then((unlisten) => unlisten).catch(console.log);
            unlisten2.then((unlisten) => unlisten).catch(console.log);
        };
    }, [reloadScene]);

    useEffect(() => {
        if (!renderingState) return;

        var animationFrameId: number;
        const render = () => {
            drawScene(renderingState.gl);
            animationFrameId = window.requestAnimationFrame(render);
        };
        render();
        return () => window.cancelAnimationFrame(animationFrameId);
    }, [drawScene]);

    useEffect(() => {
        if (!mouseDown) return;

        const onMouseMove = (e: MouseEvent) => {
            if (mouseDown) {
                const offset = [
                    mousePos.x - e.clientX,
                    -1 * (mousePos.y - e.clientY),
                ];
                setCamera((old) => moveCameraBy(old as Camera, offset));
                setMousePos({ x: e.clientX, y: e.clientY });
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
        setMousePos({ x: e.clientX, y: e.clientY });
        setMouseDown(true);

        if (!renderingData || !canvasRef.current || !camera || !textures)
            return;

        const hit = testMouse(
            e.clientX,
            e.clientY,
            canvasRef.current,
            camera,
            renderingData,
            textures,
        );
        setChEntity(hit ? { id: hit } : undefined);
    };

    return (
        <ViewportContainer ref={ref}>
            <ViewportCanvas
                onMouseDown={onMouseDown}
                onWheel={handleNavigation}
                ref={canvasRef}
                width={size.width}
                height={size.height}
            />
        </ViewportContainer>
    );
};

export default Viewport;
