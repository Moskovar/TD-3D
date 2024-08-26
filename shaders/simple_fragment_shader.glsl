 #version 330 core

in vec2 TexCoords;
uniform sampler2D terrainTexture;
in float color;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(color, color, color, 1.0);
    FragColor = texture(terrainTexture, TexCoords);
}