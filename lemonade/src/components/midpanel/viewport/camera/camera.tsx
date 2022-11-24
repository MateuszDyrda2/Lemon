import { mat4, vec3 } from 'gl-matrix';

export interface Camera {
    size: number;
    position: number[];
}

export const createCamera = (size: number) => {
    const camera = {
        size: size,
        position: [0.0, 0.0],
    };
    return camera;
};

export const moveCamera = (camera: Camera, position: number[]) => {
    camera.position = position;
};

export const getCameraModel = (camera: Camera) => {
    var model = mat4.create();

    mat4.scale(model, model, vec3.fromValues(camera.size, camera.size, 1.0));
    mat4.translate(
        model,
        model,
        vec3.fromValues(-camera.position[0], -camera.position[1], 0.0),
    );

    return model;
};

export const moveCameraBy = (camera: Camera, offset: number[]) => {
    const c: Camera = {
        size: camera.size,
        position: [
            camera.position[0] + offset[0] / camera.size,
            camera.position[1] + offset[1] / camera.size,
        ],
    };
    return c;
};

export const resetCamera = () => {
    const c: Camera = {
        size: 1,
        position: [0, 0],
    };
    return c;
};

export const zoomCamera = (camera: Camera, value: number) => {
    const c: Camera = {
        size: camera.size + value,
        position: camera.position,
    };
    return c;
};
