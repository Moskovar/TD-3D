 #version 330 core

 #define LARGETILE_SIZE 512

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool      isCellTargeted;
uniform vec3      worldPos; // Coordonnées du point d'intersection

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(texture0, TexCoords);
    vec4 color1 = texture(texture1, TexCoords);

    int wpx = int(worldPos.x), wpz = int(worldPos.z),
        x   = int(vPos.x)    , z   = int(vPos.z);

    if(isCellTargeted && (wpx / 8) == (x / 8) && (wpz / 8) == (z / 8)) 
    {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        return;
    }

    if(vPos.x > 992 && vPos.x <= 1000 || vPos.x < 1056 && vPos.x >= 1048) 
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        return;
    }

    if(int(vPos.z) % LARGETILE_SIZE >= LARGETILE_SIZE - 1 || int(vPos.x) % LARGETILE_SIZE >= LARGETILE_SIZE - 1) FragColor = mix(color0, color1, 0.5);
    else FragColor = texture(texture0, TexCoords);
}