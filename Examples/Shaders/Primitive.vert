#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);
    vertexColor = vec4(color, 1.0f);
}