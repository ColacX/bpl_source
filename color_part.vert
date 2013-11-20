
#version 150 core


in vec3 in_Vertex;
in vec4 in_Color;

// UNIFORM
uniform mat4 projection;
uniform mat4 modelview;


out vec4 color;

void main()
{
    gl_Position = projection * modelview * vec4(in_Vertex, 1.0);

    color = in_Color;
}
