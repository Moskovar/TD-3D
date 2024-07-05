#include "Bone.h"

Bone::Bone(std::vector<std::tuple<float, Vertex*>> vertices)
{
	this->vertices = vertices;
}

Bone::~Bone()
{

}

glm::mat4 Bone::interpolateTransform(double animationTime, Animation animation)
{
    const std::map<double, KeyFrame> keyFrames = animations[animation];
    //printf("KeyFrame SIZE: %d\n", keyFrames.size());

    // Si aucune keyframe n'est disponible, retourner une matrice identité
    if (keyFrames.empty()) 
    {
        return glm::mat4(1.0f);
    }

    // Trouver les keyframes qui entourent le temps d'animation donné
    auto nextKeyFrame = keyFrames.lower_bound(animationTime);

    if (nextKeyFrame == keyFrames.end()) 
    {
        // Si aucune keyframe suivante n'est trouvée, utiliser la dernière keyframe
        return glm::translate(keyFrames.rbegin()->second.position) * glm::mat4_cast(keyFrames.rbegin()->second.rotation) * glm::scale(keyFrames.rbegin()->second.scale);
        //return glm::mat4_cast(keyFrames.rbegin()->second.rotation);
        return glm::translate(keyFrames.rbegin()->second.position);
    }

    if (nextKeyFrame == keyFrames.begin()) 
    {
        // Si aucune keyframe précédente n'est trouvée, utiliser la première keyframe
        return glm::translate(keyFrames.begin()->second.position) * glm::mat4_cast(keyFrames.begin()->second.rotation) * glm::scale(keyFrames.begin()->second.scale);
        //return glm::mat4_cast(keyFrames.begin()->second.rotation);
        return glm::translate(keyFrames.begin()->second.position);
    }

    auto prevKeyFrame = std::prev(nextKeyFrame);

    //printf("AnimTime: %f ... KeyFramesSize: %d ... prev: %f ... next: %f\n", animationTime, keyFrames.size(), prevKeyFrame->first, nextKeyFrame->first);

    // Calculer le facteur d'interpolation
    float factor = static_cast<float>((animationTime - prevKeyFrame->first) / (nextKeyFrame->first - prevKeyFrame->first));

    // Interpoler les positions, rotations et échelles
    glm::vec3 position = interpolate(prevKeyFrame->second.position, nextKeyFrame->second.position, factor);
    glm::quat rotation = glm::slerp(prevKeyFrame->second.rotation, nextKeyFrame->second.rotation, factor);
    glm::vec3 scale    = interpolate(prevKeyFrame->second.scale, nextKeyFrame->second.scale, factor);

    // Créer et retourner la matrice de transformation finale
    //return glm::scale(scale) * glm::mat4_cast(rotation) * glm::translate(position);
    return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
    //return glm::mat4_cast(rotation);
    //return glm::translate(glm::mat4(1.0f), position);
}

void Bone::applyTransformations(glm::mat4 transformMatrix)
{
    for (auto vertexTuple : vertices)
    {
        float   weight = std::get<0>(vertexTuple);
        Vertex* vertex = std::get<1>(vertexTuple);

        if (weight > 0 && vertex)
        {
            // Appliquer la transformation à la position avec le poids
            glm::vec4 transformedPosition = transformMatrix * glm::vec4(vertex->position, 1.0f);
            vertex->position = glm::vec3(glm::mix(glm::vec4(vertex->position, 1.0f), transformedPosition, weight));

            // Appliquer la transformation à la normale avec le poids (sans translation)
            glm::vec4 transformedNormal = transformMatrix * glm::vec4(vertex->normal, 0.0f);
            vertex->normal = glm::normalize(glm::mix(glm::vec4(vertex->normal, 0.0f), transformedNormal, weight));
        }
    }
}
