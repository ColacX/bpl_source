#version 330
uniform sampler2D sampler0;
out vec4 frag_color;
in vec4 c;

void main()
{
	frag_color = c;
	frag_color = vec4(1, 0, 0, 1);
}
