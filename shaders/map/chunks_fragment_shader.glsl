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

    if (int(vPos.z) % 512 >= 511 && int(vPos.x) % 512 >= 511) FragColor = (color0 + color1 + color2 + color3) / 4;
    else if (int(vPos.z) % 512 >= 511)  
         if (int(vPos.x) % 1024 < 511)  FragColor = mix(color0, color2, 0.5);
         else                           FragColor = mix(color1, color3, 0.5);
    else if (int(vPos.x) % 512 >= 511)  
         if (int(vPos.z) % 1024 < 511)  FragColor = mix(color0, color1, 0.5);
         else                           FragColor = mix(color2, color3, 0.5);
    else FragColor = texture(textures[0], TexCoords);
    //FragColor = texture(textures[1], TexCoords);
}