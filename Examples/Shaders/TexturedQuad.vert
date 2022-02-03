#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec4 vertexColor;
out vec2 uv;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);
    vertexColor = color;
    uv = texCoord;
}