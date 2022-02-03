#version 430 core
layout (location = 0) in vec2 vert;
out vec2 pixelPos;

void main()
{
    gl_Position = vec4(vert.x, vert.y, 0.0f, 1.0f);
    pixelPos = vert;
}