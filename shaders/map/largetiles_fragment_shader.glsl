 #version 330 core

 #define LARGETILE_SIZE 512

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 worldPos; // Coordonnées du point d'intersection

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(texture0, TexCoords);
    vec4 color1 = texture(texture1, TexCoords);

    int wpx = int(worldPos.x), wpz = int(worldPos.z),
        x   = int(vPos.x)    , z   = int(vPos.z);

    if((wpx / 8) == (x / 8) && (wpz / 8) == (z / 8)) 
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    if(vPos.x > 999 && vPos.x <= 1004 || vPos.x < 1049 && vPos.x >= 1044) 
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    if(int(vPos.z) % LARGETILE_SIZE >= LARGETILE_SIZE - 1 || int(vPos.x) % LARGETILE_SIZE >= LARGETILE_SIZE - 1) FragColor = mix(color0, color1, 0.5);
    else FragColor = texture(texture0, TexCoords);
}