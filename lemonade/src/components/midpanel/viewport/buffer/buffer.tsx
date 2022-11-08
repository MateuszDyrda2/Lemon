export const initializeBuffers = (gl: WebGLRenderingContext) => {
    const buffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    const vertices = [50, 50, -50, 50, 50, -50, -50, -50];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

    return buffer;
};
