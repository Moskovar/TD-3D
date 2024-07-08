#version 330 core

#define NUM_BONES 100

layout(location = 0) in vec3  pos; // Position des sommets
layout(location = 5) in ivec4 boneIDs;
layout(location = 6) in vec4  boneWeights;

out vec4 vertexColor;

uniform mat4 model; // Matrice de modele
uniform mat4 view;  // Matrice de vue
uniform mat4 projection; // Matrice de projection
uniform mat4 bonesTransform[NUM_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0);

    for (int i = 0; i < 4; i++)
    {
        if (boneIDs[i] != -1) // Ignorer les IDs d'os non valides
        {
            mat4 boneTransform = bonesTransform[boneIDs[i]];
            totalPosition += boneTransform * vec4(pos, 1.0) * boneWeights[i];
        }
    }

    if (boneIDs[0] == -1) vertexColor = vec4(1.0, 0.0, 0.0, 0.0);
    if (boneWeights[0] == 0) vertexColor = vec4(0.0, 1.0, 0.0, 0.0);
    //gl_Position = projection * view * model * vec4(pos, 1.0);
    gl_Position = projection * view * model * totalPosition;
    //gl_Position = projection * view * model * vec4(pos, 1.0);
}