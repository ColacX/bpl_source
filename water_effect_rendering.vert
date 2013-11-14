#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 transformation;
out vec2 st;

void main()
{
	gl_Position = vec4(position, 0, 1);
	st = texcoord;
}
