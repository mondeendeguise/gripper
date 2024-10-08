#version 330 core

in vec3 v_pos;
in vec4 v_color;

out vec4 color;

uniform mat4 mvp;

void main()
{
    // color = v_color;
    gl_Position = mvp * vec4(v_pos, 1.0);
    color = vec4((v_pos + 0.5) * 2, 1.0);
}
