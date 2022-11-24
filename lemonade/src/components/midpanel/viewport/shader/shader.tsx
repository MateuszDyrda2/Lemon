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
        vec4 color = texture2D(uSampler, vTextureCoord);
        if (color.a <= 0.04)
            discard;

        gl_FragColor = color;
    }
`;

export const highlightFragmentSource = `
    precision mediump float;

    varying highp vec2 vTextureCoord;

    uniform sampler2D uSampler;

    void main() {
        vec4 color = vec4(1,0.8,0,0);
        vec4 alpha = vec4(0,0,0,1) * texture2D(uSampler, vTextureCoord);
        gl_FragColor = color + alpha;
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
    hProgram: WebGLProgram;
    attribLocations: Record<string, number>;
    uniformLocations: Record<string, WebGLUniformLocation | null>;
    hAttribLocations: Record<string, number>;
    hUniformLocations: Record<string, WebGLUniformLocation | null>;
}

export const initializeShaders = (
    gl: WebGLRenderingContext,
    vertexSource: string,
    fragmentSource: string,
    highlightFragmentSource: string,
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

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        console.log(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                shaderProgram,
            )}`,
        );
        return null;
    }

    // highlight
    const hVertexShader = loadShader(gl, gl.VERTEX_SHADER, vertexSource);
    const hFragmentShader = loadShader(
        gl,
        gl.FRAGMENT_SHADER,
        highlightFragmentSource,
    );

    if (hVertexShader === null || hFragmentShader === null) return null;

    const hShaderProgram = gl.createProgram();
    if (hShaderProgram === null) return null;

    gl.attachShader(hShaderProgram, hVertexShader);
    gl.attachShader(hShaderProgram, hFragmentShader);
    gl.linkProgram(hShaderProgram);

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
        hProgram: hShaderProgram,
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
        hAttribLocations: {
            vertexPosition: gl.getAttribLocation(hShaderProgram, 'position'),
            textureCoords: gl.getAttribLocation(hShaderProgram, 'texCoords'),
        },
        hUniformLocations: {
            projectionMatrix: gl.getUniformLocation(
                hShaderProgram,
                'projection',
            ),
            modelViewMatrix: gl.getUniformLocation(hShaderProgram, 'model'),
            uSampler: gl.getUniformLocation(hShaderProgram, 'uSampler'),
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
