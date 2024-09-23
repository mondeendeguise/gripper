#version 330 core

in vec3 v_pos;
in vec4 v_color;

out vec4 color;

uniform mat4 model;

void main()
{
    color = v_color;
    gl_Position = model * vec4(v_pos, 1.0);
}
