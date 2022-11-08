export const vertexSource = `
    attribute vec4 a_position;
    
    uniform mat4 model;
    uniform mat4 projection;
    
    void main() {
        gl_Position = projection * model * a_position;
    }
`;

export const fragmentSource = `
    precision mediump float;

    void main() {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
`;

const loadShader = (
    gl: WebGLRenderingContext,
    type: number,
    source: string,
) => {
    const shader = gl.createShader(type);
    if (shader === null) return null;

    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        console.log(
            `An error occured compiling the shaders: ${gl.getShaderInfoLog(
                shader,
            )}`,
        );
        gl.deleteShader(shader);
        return null;
    }

    return shader;
};

export const initializeShaders = (
    gl: WebGLRenderingContext,
    vertexSource: string,
    fragmentSource: string,
) => {
    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vertexSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fragmentSource);

    if (vertexShader === null || fragmentShader === null) return null;

    const shaderProgram = gl.createProgram();
    if (shaderProgram === null) return null;

    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        console.log(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                shaderProgram,
            )}`,
        );
        return null;
    }

    return shaderProgram;
};
