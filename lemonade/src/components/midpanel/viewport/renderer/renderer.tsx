import { glMatrix, mat4, vec3 } from 'gl-matrix';
import { RenderingData } from '../../../../props/rendering_data';
import { Texture } from '../assets/assets';
import { Buffers } from '../buffer/buffer';
import { ProgramInfo } from '../shader/shader';

const renderScene = (
    renderingData: RenderingData[],
    gl: WebGLRenderingContext,
    program: ProgramInfo,
    buffer: Buffers,
    textures: { [nameid: number]: Texture },
    width: number,
    height: number,
) => {
    const projection = mat4.create();
    const hWidth = width * 0.5;
    const hHeight = height * 0.5;
    mat4.ortho(projection, -hWidth, hWidth, -hHeight, hHeight, -1, 1);
    gl.useProgram(program.program);
    gl.uniformMatrix4fv(
        program.uniformLocations.projectionMatrix,
        false,
        projection,
    );
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

        const model = mat4.fromScaling(
            data.model,
            vec3.fromValues(size[0], size[1], 1),
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
    }
};

export default renderScene;
