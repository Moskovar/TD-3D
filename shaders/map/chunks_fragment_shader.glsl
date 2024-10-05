 #version 330 core

#define LARGETILE_SIZE 512//= taille de la texture

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D textures[4];

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(textures[0], TexCoords);
    vec4 color1 = texture(textures[1], TexCoords);
    vec4 color2 = texture(textures[2], TexCoords);
    vec4 color3 = texture(textures[3], TexCoords);

    int   xCoord = int(TexCoords.x * LARGETILE_SIZE),
          yCoord = int(TexCoords.y * LARGETILE_SIZE);

    if (int(vPos.z) % LARGETILE_SIZE >= LARGETILE_SIZE - 1 && int(vPos.x) % LARGETILE_SIZE >= LARGETILE_SIZE - 1) FragColor =  (texelFetch(textures[0], ivec2(0, LARGETILE_SIZE - 1), 0) + texelFetch(textures[1], ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0)  +
                                                                            texelFetch(textures[2], ivec2(0, 0)  , 0) + texelFetch(textures[3], ivec2(LARGETILE_SIZE - 1, 0)  , 0)) / 4.0f;
    else if (int(vPos.z) % LARGETILE_SIZE >= LARGETILE_SIZE - 1)  
         if (int(vPos.x) % (2 * LARGETILE_SIZE) < LARGETILE_SIZE - 1)  FragColor = mix(texelFetch(textures[0], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[2], ivec2(xCoord, 0), 0), 0.5);
         else                           FragColor = mix(texelFetch(textures[1], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[3], ivec2(xCoord, 0), 0), 0.5);
    else if (int(vPos.x) % LARGETILE_SIZE >= LARGETILE_SIZE - 1)  
         if (int(vPos.z) % (2 * LARGETILE_SIZE) < LARGETILE_SIZE - 1)  FragColor = mix(texelFetch(textures[0], ivec2(0, yCoord), 0), texelFetch(textures[1], ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
         else                           FragColor = mix(texelFetch(textures[2], ivec2(0, yCoord), 0), texelFetch(textures[3], ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
    else FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}