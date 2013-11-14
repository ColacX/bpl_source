//fbo ping pong with two textures
//the current pixel on the texture will not be overwritten until the fragement shader has run its program on it

#version 330
uniform sampler2D height0_sampler; //current
uniform sampler2D height1_sampler; //previous
in vec2 st;
out vec4 frag_color;

void main()
{
	//sample the pixel on the current frame
	float c = texture2D(height0_sampler, st).r;

	//sample slightly to the sides
	float d = 0.0007f;
	float a = 0.0f;
	a += texture2D(height1_sampler, vec2(st.s - d, st.t)).r; //left
	a += texture2D(height1_sampler, vec2(st.s + d, st.t)).r; //right
	a += texture2D(height1_sampler, vec2(st.s, st.t - d)).r; //up
	a += texture2D(height1_sampler, vec2(st.s, st.t + d)).r; //down
	
	////tweak the parameters a bit
	c = a*0.5f - c;
	c = c * 0.98f;
	c = clamp(c, -0.8, 0.8);
	frag_color = vec4(c, 0, 0, 1);
}
