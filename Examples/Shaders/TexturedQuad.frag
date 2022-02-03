#version 330 core
in vec4 vertexColor;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D s;

void main()
{
    vec4 color = vertexColor * texture(s, uv);

    if (color.a < 0.1)
        discard;

    fragColor = color;
}