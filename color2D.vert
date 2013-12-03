
#version 150 core


in vec2 in_Vertex;
in vec3 in_Color;

out vec3 color;


void main()
{
    gl_Position = vec4(in_Vertex, 0.0, 1.0);

    color = in_Color;
}
