import { readBinaryFile } from '@tauri-apps/api/fs';

export interface Texture {
    path: string;
    handle: WebGLTexture | null;
    size: number[];
}

export interface TextureDefinition {
    path: string;
    width: number;
    height: number;
}

const load_textures = async (
    gl: WebGLRenderingContext,
    definitions: { [key: number]: TextureDefinition },
) => {
    var textures: { [nameid: number]: Texture } = {};

    for (const [nameid, def] of Object.entries(definitions)) {
        const tex = await load_texture(gl, def);
        textures[parseInt(nameid)] = {
            path: def.path,
            handle: tex,
            size: [def.width, def.height],
        };
    }

    return textures;
};

const arrayBufferToBase64 = (buffer: Uint8Array) => {
    var binary = '';
    var bytes = new Uint8Array(buffer);
    var len = bytes.byteLength;
    for (var i = 0; i < len; ++i) {
        binary += String.fromCharCode(bytes[i]);
    }
    return window.btoa(binary);
};

const load_texture = async (
    gl: WebGLRenderingContext,
    textureDefinition: TextureDefinition,
) => {
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);

    const internalFormat = gl.RGBA;
    const sourceFormat = gl.RGBA;
    const srcType = gl.UNSIGNED_BYTE;
    const { path, width, height } = textureDefinition;

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);

    const tempImage = new Uint8Array(width * height * 4);
    for (var i = 0; i < width * height * 4; i = i + 4) {
        tempImage[i + 0] = 128;
        tempImage[i + 1] = 0;
        tempImage[i + 2] = 128;
        tempImage[i + 3] = 255;
    }

    gl.texImage2D(
        gl.TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        sourceFormat,
        srcType,
        tempImage,
    );

    try {
        const response = await readBinaryFile(path);
        var base64 = arrayBufferToBase64(response);
        var image = new Image();
        image.src = 'data:image/png;base64,' + base64;
        await image.decode();
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(
            gl.TEXTURE_2D,
            0,
            internalFormat,
            sourceFormat,
            srcType,
            image,
        );
        gl.bindTexture(gl.TEXTURE_2D, null);
    } catch (e) {
        console.log('Error decoding image: ' + e);
    }

    return texture;
};

export default load_textures;
