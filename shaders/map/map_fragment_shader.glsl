 #version 330 core

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(texture0, TexCoords);
    vec4 color1 = texture(texture1, TexCoords);
    vec4 color2 = texture(texture2, TexCoords);
    vec4 color3 = texture(texture3, TexCoords);

    if (int(vPos.z) % 512 >= 511 && int(vPos.x) % 512 >= 511) FragColor = (color0 + color1 + color2 + color3) / 4;
    else if (int(vPos.z) % 512 >= 511)  
         if (int(vPos.x) % 1024 < 511)  FragColor = mix(color0, color2, 0.5);
         else                           FragColor = mix(color1, color3, 0.5);
    else if (int(vPos.x) % 512 >= 511)  
         if (int(vPos.z) % 1024 < 511)  FragColor = mix(color0, color1, 0.5);
         else                           FragColor = mix(color2, color3, 0.5);
    else FragColor = texture(texture0, TexCoords);
    //FragColor = texture(texture0, TexCoords);
}