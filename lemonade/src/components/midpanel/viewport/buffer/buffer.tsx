export interface Buffers {
    vertexBuffer: WebGLBuffer;
    texBuffer: WebGLBuffer;
}

export const initializeBuffers = (gl: WebGLRenderingContext) => {
    const buffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    const vertices = [
        -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5,
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

    const texBuffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, texBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(16), gl.STREAM_DRAW);

    if (buffer === null || texBuffer === null) {
        console.log('One of the buffers is null');
        return null;
    }

    const buffers: Buffers = { vertexBuffer: buffer, texBuffer: texBuffer };
    return buffers;
};
