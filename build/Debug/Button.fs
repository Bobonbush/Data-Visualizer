#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
void main()
{
    vec4 texcolor = texture(ourTexture, TexCoord);
    if(texcolor.a < 0.1)
        discard;

    FragColor = texcolor;
}