#version 330
#ifdef __VERT__
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
out vec2 st;

void main()
{
	gl_Position = vec4(position, 0, 1);
	st = texcoord;
}
#endif

#ifdef __FRAG__
uniform sampler2D sampler0;

in vec2 st;
out vec4 frag_color;

void main()
{
	frag_color = texture2D(sampler0, st);
}

#endif
