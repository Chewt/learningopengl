#version 330 core
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D mytexture;

out vec4 FragColor;

void main()
{
    float r = Color.x;
    float g = Color.y;
    float b = Color.z;
    
    if (r < 0)
        r = -r;
    if (g < 0)
        g = -g;
    if (b < 0)
        b = -b;

    FragColor  = texture(mytexture, TexCoord);
    //FragColor = vec4(r, g, b, 1.f);
}
