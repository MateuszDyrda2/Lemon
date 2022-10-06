import "../styles/viewport.scss";

import {
  useCallback,
  useEffect,
  useLayoutEffect,
  useRef,
  useState,
} from "react";
import { mat4 } from "gl-matrix";
import { useResizeDetector } from "react-resize-detector";

const vertex_source = `
  attribute vec4 a_position;

  uniform mat4 model;
  uniform mat4 projection;

  void main() {
    gl_Position = projection * model * a_position;
  }
`;
const fragment_source = `
  precision mediump float;

  void main() {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
`;
const loadShader = (
  gl: WebGLRenderingContext,
  type: number,
  source: string
) => {
  const shader = gl.createShader(type);
  if (shader === null) return null;

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.log(
      `An error occured compling the shaders: ${gl.getShaderInfoLog(shader)}`
    );
    gl.deleteShader(shader);
    return null;
  }
  return shader;
};

const initShaders = (gl: WebGLRenderingContext) => {
  const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vertex_source);
  const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fragment_source);
  if (vertexShader === null || fragmentShader === null) return null;

  const shaderProgram = gl.createProgram();
  if (shaderProgram === null) return null;

  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    console.log(
      `Unable to initialize the shader program: ${gl.getProgramInfoLog(
        shaderProgram
      )}`
    );
    return null;
  }

  return shaderProgram;
};

const initBuffers = (gl: WebGLRenderingContext) => {
  const buffer = gl.createBuffer();

  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  const vertices = [50, 50, -50, 50, 50, -50, -50, -50];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

  return buffer;
};

interface rendering_data {
  buffer: WebGLBuffer;
  program: WebGLProgram;
}

const Viewport = () => {
  const canvas = useRef<HTMLCanvasElement>(null);
  const [renderingData, setRenderingData] = useState<rendering_data>();
  const { width, height, ref } = useResizeDetector();

  const drawScene = useCallback(
    (gl: WebGLRenderingContext) => {
      if (!width || !height) return;

      gl.clearColor(0.0, 0.0, 0.0, 1.0);
      gl.clear(gl.COLOR_BUFFER_BIT);
      gl.viewport(0, 0, width, height);

      if (!renderingData) return;
      const program = renderingData.program;
      const buffer = renderingData.buffer;

      const vertexPos = gl.getAttribLocation(program, "a_position");
      const projectionPos = gl.getUniformLocation(program, "projection");
      const modelPos = gl.getUniformLocation(program, "model");
      if (projectionPos === null || modelPos === null) return;

      const projection = mat4.create();
      const hWidth = width * 0.5;
      const hHeight = height * 0.5;
      mat4.ortho(projection, -hWidth, hWidth, -hHeight, hHeight, -1, 1);
      const model = mat4.create();

      gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
      gl.vertexAttribPointer(vertexPos, 2, gl.FLOAT, false, 0, 0);
      gl.enableVertexAttribArray(vertexPos);
      gl.useProgram(program);
      gl.uniformMatrix4fv(projectionPos, false, projection);
      gl.uniformMatrix4fv(modelPos, false, model);
      gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
    },
    [renderingData, width, height]
  );

  useLayoutEffect(() => {
    if (canvas.current === null) return;

    const gl = canvas.current.getContext("webgl");
    if (gl === null) return;

    const shaderProgram = initShaders(gl);
    if (shaderProgram === null) return;

    const buffer = initBuffers(gl);
    if (buffer === null) return;

    setRenderingData({ buffer: buffer, program: shaderProgram });
  }, []);

  useEffect(() => {
    if (canvas.current === null) return;

    const gl = canvas.current.getContext("webgl");
    if (gl === null) return;

    var animationFrameId: number;
    const render = () => {
      drawScene(gl);
      animationFrameId = window.requestAnimationFrame(render);
    };
    render();
    return () => window.cancelAnimationFrame(animationFrameId);
  }, [drawScene]);

  return (
    <div className="viewport" ref={ref}>
      <canvas ref={canvas} width={width} height={height} />
    </div>
  );
};

export default Viewport;
