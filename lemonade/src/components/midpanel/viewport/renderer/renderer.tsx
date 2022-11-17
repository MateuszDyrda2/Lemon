import { mat4, vec3 } from 'gl-matrix';
import { RenderingData } from '../../../../props/rendering_data';
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
) => {
    const projection = mat4.create();
    const hWidth = width * 0.5;
    const hHeight = height * 0.5;
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
        const texCoords = data.texCoords;

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

            const texCoords = found.texCoords;

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
};

export default renderScene;
