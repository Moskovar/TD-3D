#version 330 core

in vec4 vertexColor;

out vec4 FragColor;

uniform vec3 color; // Couleur uniforme

void main()
{
    if(vertexColor.x != 0) FragColor = vertexColor;
    else FragColor = vec4(color, 1.0); // Définir la couleur du fragment
}