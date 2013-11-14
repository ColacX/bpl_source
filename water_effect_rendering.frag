#version 330
uniform sampler2D background_sampler;

in vec2 st; //0 to 1
out vec4 frag_color;

void main()
{
	vec4 texel_background = texture2D(background_sampler, st); //displaced background
	//frag_color = vec4(1, 0, 0, 0);
	frag_color = texel_background;
}
