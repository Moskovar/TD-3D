#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out float color;

uniform mat4 model; // Matrice de modele
uniform mat4 view;  // Matrice de vue
uniform mat4 projection; // Matrice de projection

void main() 
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    color = pos.y/255.0f + 0.2;
    TexCoords = texCoords;
}