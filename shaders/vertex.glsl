#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoordin;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Color;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.f);
    Color = Pos;
    TexCoord = TexCoordin;
}
