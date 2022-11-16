export const vertexSource = `
    attribute vec2 position;
    attribute vec2 texCoords;
    
    uniform mat4 model;
    uniform mat4 projection;

    varying highp vec2 vTextureCoord;
    
    void main() {
        gl_Position = projection * model * vec4(position, 0.0, 1.0);
        vTextureCoord = texCoords;
    }
`;

export const fragmentSource = `
    precision mediump float;

    varying highp vec2 vTextureCoord;

    uniform sampler2D uSampler;

    void main() {
        gl_FragColor = texture2D(uSampler, vTextureCoord);
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

export interface ProgramInfo {
    program: WebGLProgram;
    attribLocations: Record<string, number>;
    uniformLocations: Record<string, WebGLUniformLocation | null>;
}

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

    // enable blending
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        console.log(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                shaderProgram,
            )}`,
        );
        return null;
    }

    //return shaderProgram;
    const programInfo: ProgramInfo = {
        program: shaderProgram,
        attribLocations: {
            vertexPosition: gl.getAttribLocation(shaderProgram, 'position'),
            textureCoords: gl.getAttribLocation(shaderProgram, 'texCoords'),
        },
        uniformLocations: {
            projectionMatrix: gl.getUniformLocation(
                shaderProgram,
                'projection',
            ),
            modelViewMatrix: gl.getUniformLocation(shaderProgram, 'model'),
            uSampler: gl.getUniformLocation(shaderProgram, 'uSampler'),
        },
    };

    if (programInfo.attribLocations.vertexPosition < 0) {
        console.log('no attribute named position');
    }

    if (programInfo.attribLocations.textureCoords < 0) {
        console.log('no attribute named texCoords');
    }

    if (!programInfo.uniformLocations.projectionMatrix) {
        console.log('no attribute named projection');
    }

    if (!programInfo.uniformLocations.modelViewMatrix) {
        console.log('no attribute named model');
    }

    if (!programInfo.uniformLocations.uSampler) {
        console.log('no attribute named uSampler');
    }

    return programInfo;
};
