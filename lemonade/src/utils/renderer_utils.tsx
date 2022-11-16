export const getRelativeMousePosition = (
    e: MouseEvent | React.MouseEvent,
    canvas: HTMLCanvasElement,
) => {
    var rect = canvas.getBoundingClientRect();

    return {
        x: e.clientX - rect.left,
        y: e.clientY - rect.top,
    };
};

export const getMousePosition = (
    e: MouseEvent | React.MouseEvent,
    canvas: HTMLCanvasElement,
) => {
    var pos = getRelativeMousePosition(e, canvas);

    pos.x = (pos.x * canvas.width) / canvas.clientWidth;
    pos.y = (pos.y * canvas.height) / canvas.clientHeight;

    return pos;
};

export const getWebGLCoords = (
    e: MouseEvent | React.MouseEvent,
    canvas: HTMLCanvasElement,
) => {
    var pos = getMousePosition(e, canvas);
    const x = (pos.x / canvas.width) * 2 - 1;
    const y = (pos.y / canvas.height) * -2 + 1;
    return {
        x: x,
        y: y,
    };
};

export const getWebGLCoords2 = (
    e: MouseEvent | React.MouseEvent,
    canvas: HTMLCanvasElement,
) => {
    const x = ((e.clientX - canvas.offsetLeft) / canvas.clientWidth) * 2 - 1;
    const y = -((e.clientY - canvas.offsetTop) / canvas.clientHeight) * 2 + 1;
    return {
        x: x,
        y: y,
    };
};

export const getWebGLCoords3 = (
    e: MouseEvent | React.MouseEvent,
    canvas: HTMLCanvasElement,
) => {
    var pos = getRelativeMousePosition(e, canvas);

    pos.x = (pos.x * canvas.width) / canvas.clientWidth;
    pos.y = canvas.height - (pos.y * canvas.height) / canvas.clientHeight - 1;
    return {
        x: pos.x,
        y: pos.y,
    };
};
