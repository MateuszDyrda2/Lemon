
#if VERTEX_PROGRAM
layout (location = 0) in vec2 position; // vec2 position
layout (location = 1) in vec4 color; // vec4 normalized color
layout (location = 2) in vec2 texCoords; // vec2 texCoords

out vec2 TexCoords;
out vec4 ourColor;

uniform mat4 viewProj;

void main()
{
	gl_Position = viewProj * vec4(position, 0.0, 1.0);
	TexCoords = texCoords;
	ourColor = color;	
}
#else if FRAGMENT_PROGRAM
in vec2 TexCoords
in vec4 ourColor;

out vec4 color;

uniform sampler2D image;

void main()
{
	color = texture(image, TexCoords) * ourColor;
}
#endif