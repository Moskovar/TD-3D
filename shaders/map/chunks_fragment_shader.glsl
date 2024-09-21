 #version 330 core

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

    int   xCoord = int(TexCoords.x * 512.0f),
          yCoord = int(TexCoords.y * 512.0f);

    if (int(vPos.z) % 512 >= 511 && int(vPos.x) % 512 >= 511) FragColor =  (texelFetch(textures[0], ivec2(0, 511), 0) + texelFetch(textures[1], ivec2(511, 511), 0)  +
                                                                            texelFetch(textures[2], ivec2(0, 0)  , 0) + texelFetch(textures[3], ivec2(511, 0)  , 0)) / 4.0f;
    else if (int(vPos.z) % 512 >= 511)  
         if (int(vPos.x) % 1024 < 511)  FragColor = mix(texelFetch(textures[0], ivec2(xCoord, 511), 0), texelFetch(textures[2], ivec2(xCoord, 0), 0), 0.5);
         else                           FragColor = mix(texelFetch(textures[1], ivec2(xCoord, 511), 0), texelFetch(textures[3], ivec2(xCoord, 0), 0), 0.5);
    else if (int(vPos.x) % 512 >= 511)  
         if (int(vPos.z) % 1024 < 511)  FragColor = mix(texelFetch(textures[0], ivec2(0, yCoord), 0), texelFetch(textures[1], ivec2(511, yCoord), 0), 0.5);
         else                           FragColor = mix(texelFetch(textures[2], ivec2(0, yCoord), 0), texelFetch(textures[3], ivec2(511, yCoord), 0), 0.5);
    else FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}