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

export const colliderVertexSource = `
    attribute vec2 position;

    uniform mat4 projection;

    void main() {
        gl_Position = projection * vec4(position, 1.0, 1.0);
    }
`;

export const colliderFragmentSource = `
    precision mediump float;

    void main() {
        gl_FragColor = vec4(1.0, 0.2, 0.2, 1.0);
    }
`;
