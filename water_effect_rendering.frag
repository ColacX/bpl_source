#version 330
uniform sampler2D background_sampler;
uniform sampler2D height_sampler;

in vec2 st;
out vec4 frag_color;

void main()
{
	//sample the diffrence in height horisontally and verticaly
	float d = 0.0007f;
	float dx = texture2D(height_sampler, vec2(st.s - d, st.t)).r - texture2D(height_sampler, vec2(st.s + d, st.t)).r;
	float dy = texture2D(height_sampler, vec2(st.s, st.t - d)).r - texture2D(height_sampler, vec2(st.s, st.t + d)).r;
	frag_color = texture2D(background_sampler, st + vec2(dx, dy));

	//frag_color = texture2D(background_sampler, st);
	//frag_color = texture2D(height_sampler, st);
}
