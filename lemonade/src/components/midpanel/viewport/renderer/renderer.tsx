import { mat4, vec3, vec4 } from 'gl-matrix';
import { DebugData, RenderingData } from '../../../../props/rendering_data';
import { Entity } from '../../../../state/chosen_entity';
import { Texture } from '../assets/assets';
import { Buffers } from '../buffer/buffer';
import { Camera, getCameraModel } from '../camera/camera';
import { ProgramInfo } from '../shader/shader';

const renderScene = (
    renderingData: RenderingData[],
    gl: WebGLRenderingContext,
    program: ProgramInfo,
    buffer: Buffers,
    camera: Camera,
    textures: { [nameid: number]: Texture },
    width: number,
    height: number,
    chosen: Entity | undefined,
    debug: boolean,
    debugData: DebugData[],
) => {
    const projection = mat4.create();

    const aspect = width / height;
    const hHeight = height * 0.5;
    const hWidth = hHeight * aspect;

    mat4.ortho(projection, -hWidth, hWidth, -hHeight, hHeight, -1, 1);
    gl.useProgram(program.program);
    var view = getCameraModel(camera);

    mat4.multiply(view, projection, view);

    gl.uniformMatrix4fv(program.uniformLocations.projectionMatrix, false, view);
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer.vertexBuffer);
    gl.vertexAttribPointer(
        program.attribLocations.vertexPosition,
        2,
        gl.FLOAT,
        false,
        0,
        0,
    );
    gl.enableVertexAttribArray(program.attribLocations.vertexPosition);

    for (const data of renderingData) {
        const { handle, size } = textures[data.textureid];
        const texCoords = data.tex_coords;

        const texSize = [
            size[0] * (texCoords[2] - texCoords[0]),
            size[1] * (texCoords[3] - texCoords[1]),
        ];

        var model = mat4.create();
        mat4.scale(
            model,
            data.model,
            vec3.fromValues(texSize[0], texSize[1], 1),
        );

        const coords = [
            texCoords[0],
            texCoords[1],
            texCoords[2],
            texCoords[1],
            texCoords[2],
            texCoords[3],
            texCoords[2],
            texCoords[3],
            texCoords[0],
            texCoords[3],
            texCoords[0],
            texCoords[1],
        ];

        const isChosen = chosen && chosen.id === data.nameid;

        if (isChosen) {
            gl.stencilFunc(gl.ALWAYS, 1, 0xff);
            gl.stencilMask(0xff);
        }
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer.texBuffer);
        gl.bufferSubData(gl.ARRAY_BUFFER, 0, new Float32Array(coords));
        gl.vertexAttribPointer(
            program.attribLocations.textureCoords,
            2,
            gl.FLOAT,
            false,
            0,
            0,
        );
        gl.enableVertexAttribArray(program.attribLocations.textureCoords);
        gl.uniformMatrix4fv(
            program.uniformLocations.modelViewMatrix,
            false,
            model,
        );
        gl.bindTexture(gl.TEXTURE_2D, handle);
        gl.drawArrays(gl.TRIANGLES, 0, 6);

        if (isChosen) {
            gl.stencilMask(0x00);
        }
    }

    if (chosen) {
        const found = renderingData.find((data) => data.nameid === chosen.id);
        if (found) {
            gl.stencilFunc(gl.NOTEQUAL, 1, 0xff);
            gl.stencilMask(0x00);
            gl.disable(gl.DEPTH_TEST);
            const { handle, size } = textures[found.textureid];

            gl.useProgram(program.hProgram);
            gl.uniformMatrix4fv(
                program.hUniformLocations.projectionMatrix,
                false,
                view,
            );
            gl.bindBuffer(gl.ARRAY_BUFFER, buffer.vertexBuffer);
            gl.vertexAttribPointer(
                program.hAttribLocations.vertexPosition,
                2,
                gl.FLOAT,
                false,
                0,
                0,
            );
            gl.enableVertexAttribArray(program.hAttribLocations.vertexPosition);

            const texCoords = found.tex_coords;

            const texSize = [
                size[0] * (texCoords[2] - texCoords[0]) + 5,
                size[1] * (texCoords[3] - texCoords[1]) + 5,
            ];

            var model = mat4.create();
            mat4.scale(
                model,
                found.model,
                vec3.fromValues(texSize[0], texSize[1], 1),
            );

            const coords = [
                texCoords[0],
                texCoords[1],
                texCoords[2],
                texCoords[1],
                texCoords[2],
                texCoords[3],
                texCoords[2],
                texCoords[3],
                texCoords[0],
                texCoords[3],
                texCoords[0],
                texCoords[1],
            ];

            gl.bindBuffer(gl.ARRAY_BUFFER, buffer.texBuffer);
            gl.bufferSubData(gl.ARRAY_BUFFER, 0, new Float32Array(coords));
            gl.vertexAttribPointer(
                program.hAttribLocations.textureCoords,
                2,
                gl.FLOAT,
                false,
                0,
                0,
            );
            gl.enableVertexAttribArray(program.hAttribLocations.textureCoords);
            gl.uniformMatrix4fv(
                program.hUniformLocations.modelViewMatrix,
                false,
                model,
            );
            gl.bindTexture(gl.TEXTURE_2D, handle);
            gl.drawArrays(gl.TRIANGLES, 0, 6);

            gl.stencilMask(0xff);
            gl.stencilFunc(gl.ALWAYS, 1, 0xff);
            gl.enable(gl.DEPTH_TEST);
        }
    }

    if (debug) {
        gl.useProgram(program.dProgram);
        gl.uniformMatrix4fv(
            program.dUniformLocations.projectionMatrix,
            false,
            view,
        );
        const buffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(10), gl.STREAM_DRAW);
        if (buffer === null) console.log('null buffer');

        gl.vertexAttribPointer(
            program.dAttribLocations.vertexPosition,
            2,
            gl.FLOAT,
            false,
            0,
            0,
        );
        gl.enableVertexAttribArray(program.dAttribLocations.vertexPosition);

        for (var i = 0; i < debugData.length; ++i) {
            gl.bufferSubData(
                gl.ARRAY_BUFFER,
                0,
                new Float32Array(debugData[i].coords),
            );
            gl.drawArrays(gl.LINE_STRIP, 0, 5);
        }
    }
};

export const testMouse = (
    x: number,
    y: number,
    canvas: HTMLCanvasElement,
    camera: Camera,
    renderingData: RenderingData[],
    textures: { [nameid: number]: Texture },
) => {
    var rect = canvas.getBoundingClientRect();

    var relativeMousePos = {
        x: x - rect.left,
        y: y - rect.top,
    };

    relativeMousePos.x =
        (relativeMousePos.x * canvas.width) / canvas.clientWidth;
    relativeMousePos.y =
        (relativeMousePos.y * canvas.height) / canvas.clientHeight;

    const webglCoords = {
        x: (relativeMousePos.x / canvas.width) * 2 - 1,
        y: (relativeMousePos.y / canvas.height) * -2 + 1,
    };

    const hWidth = canvas.width * 0.5;
    const hHeight = canvas.height * 0.5;

    const gameWorldCoords = {
        x: (webglCoords.x * hWidth) / camera.size + camera.position[0],
        y: (webglCoords.y * hHeight) / camera.size + camera.position[1],
    };

    const pointer = vec4.fromValues(gameWorldCoords.x, gameWorldCoords.y, 0, 0);

    for (var i = 0; i < renderingData.length; ++i) {
        var model = renderingData[i].model;
        var position = vec3.create();
        var scale = vec3.create();

        mat4.getTranslation(position, model);
        mat4.getScaling(scale, model);

        const { size } = textures[renderingData[i].textureid];
        const texCoords = renderingData[i].tex_coords;

        const texSize = [
            size[0] * (texCoords[2] - texCoords[0]) * 0.5,
            size[1] * (texCoords[3] - texCoords[1]) * 0.5,
        ];

        const leftBound = position[0] - texSize[0] * scale[0];
        const rightBound = position[0] + texSize[0] * scale[0]!;
        const bottomBound = position[1] - texSize[1] * scale[1];
        const topBound = position[1] + texSize[1] * scale[1];

        if (
            pointer[0] >= leftBound &&
            pointer[0] <= rightBound &&
            pointer[1] >= bottomBound &&
            pointer[1] <= topBound
        ) {
            return renderingData[i].nameid;
        }
    }
};

export default renderScene;
