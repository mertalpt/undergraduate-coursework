#version 400
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec4 vertex_pos;
layout(location = 1) in vec3 vertex_col;
layout(location = 2) uniform mat4 model_view;
layout(location = 3) uniform mat4 projection;

out vec3 color;

void main()
{
    gl_Position = projection * model_view * vertex_pos;
    color = vertex_col;
}
