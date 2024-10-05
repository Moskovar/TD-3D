 #version 330 core

 #define LARGETILE_SIZE 512

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(texture0, TexCoords);
    vec4 color1 = texture(texture1, TexCoords);

    if(int(vPos.z) % LARGETILE_SIZE >= LARGETILE_SIZE - 1 || int(vPos.x) % LARGETILE_SIZE >= LARGETILE_SIZE - 1) FragColor = mix(color0, color1, 0.5);
    else FragColor = texture(texture0, TexCoords);
}