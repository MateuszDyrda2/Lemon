#version 330 core

in vec2 TexCoords;
in vec4 ourColor;

out vec4 color;

uniform sampler2D image;

void main()
{
	color = texture(image, TexCoords) * ourColor;
}