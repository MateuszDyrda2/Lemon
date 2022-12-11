import {
    vertexSource,
    fragmentSource,
    highlightFragmentSource,
    colliderVertexSource,
    colliderFragmentSource,
} from '../../../../props/shaders';

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
    dProgram: WebGLProgram;
    attribLocations: Record<string, number>;
    uniformLocations: Record<string, WebGLUniformLocation | null>;
    hAttribLocations: Record<string, number>;
    hUniformLocations: Record<string, WebGLUniformLocation | null>;
    dAttribLocations: Record<string, number>;
    dUniformLocations: Record<string, WebGLUniformLocation | null>;
}

export const initializeShaders = (gl: WebGLRenderingContext) => {
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

    if (!gl.getProgramParameter(hShaderProgram, gl.LINK_STATUS)) {
        console.log(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                hShaderProgram,
            )}`,
        );
        return null;
    }

    // debug
    const dVertexShader = loadShader(
        gl,
        gl.VERTEX_SHADER,
        colliderVertexSource,
    );
    const dFragmentShader = loadShader(
        gl,
        gl.FRAGMENT_SHADER,
        colliderFragmentSource,
    );

    if (!dVertexShader || !dFragmentShader) return null;

    const dShaderProgram = gl.createProgram();
    if (!dShaderProgram) return null;

    gl.attachShader(dShaderProgram, dVertexShader);
    gl.attachShader(dShaderProgram, dFragmentShader);
    gl.linkProgram(dShaderProgram);

    if (!gl.getProgramParameter(dShaderProgram, gl.LINK_STATUS)) {
        console.log(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                dShaderProgram,
            )}`,
        );
        return null;
    }

    //return shaderProgram;
    const programInfo: ProgramInfo = {
        program: shaderProgram,
        hProgram: hShaderProgram,
        dProgram: dShaderProgram,
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
        dAttribLocations: {
            vertexPosition: gl.getAttribLocation(dShaderProgram, 'position'),
        },
        dUniformLocations: {
            projectionMatrix: gl.getUniformLocation(
                dShaderProgram,
                'projection',
            ),
        },
    };

    if (programInfo.attribLocations.vertexPosition < 0) {
        console.log('no attribute named position');
    }

    if (programInfo.attribLocations.textureCoords < 0) {
        console.log('no attribute named texCoords');
    }

    if (!programInfo.uniformLocations.projectionMatrix) {
        console.log('no uniform named projection');
    }

    if (!programInfo.uniformLocations.modelViewMatrix) {
        console.log('no uniform named model');
    }

    if (!programInfo.uniformLocations.uSampler) {
        console.log('no uniform named uSampler');
    }

    if (!programInfo.dUniformLocations.projectionMatrix) {
        console.log('no uniform named projection');
    }

    if (programInfo.dAttribLocations.vertexPosition < 0) {
        console.log('no attribute named position');
    }

    return programInfo;
};
