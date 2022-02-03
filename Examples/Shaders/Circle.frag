#version 430 core
out vec4 fragColor;
in vec2 pixelPos;
layout (location = 0) uniform float radius;
layout (location = 1) uniform vec3 color;
layout (location = 2) uniform vec2 mid;
layout (location = 3) uniform float aspectRatio;

void main()
{
    float x = pixelPos.x - mid.x;
    float y = (pixelPos.y - mid.y) / aspectRatio;;
    if (sqrt(x * x + y * y) > radius)
    {
        discard;
    }

    fragColor = vec4(color, 1.0f);
}