
#ifdef VERTEX_PROGRAM
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 viewProj;
uniform mat4 model;

void main()
{
    TexCoords = texCoords;
    gl_Position = viewProj * model * vec4(position, 0.0, 1.0);
}
#endif
#ifdef FRAGMENT_PROGRAM
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    color = texture(image, TexCoords) * spriteColor;
}
#endif
