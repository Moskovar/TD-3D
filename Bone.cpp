#include "Bone.h"

Bone::Bone(int id, std::string name, aiMatrix4x4 offsetMatrix, std::vector<std::tuple<float, Vertex>> vertices)
{
    this->id = id;
    this->name = name;
    this->offetMatrix = glm::mat4
    {
        offsetMatrix.a1, offsetMatrix.b1, offsetMatrix.c1, offsetMatrix.d1,
        offsetMatrix.a2, offsetMatrix.b2, offsetMatrix.c2, offsetMatrix.d2,
        offsetMatrix.a3, offsetMatrix.b3, offsetMatrix.c3, offsetMatrix.d3,
        offsetMatrix.a4, offsetMatrix.b4, offsetMatrix.c4, offsetMatrix.d4
    };

	this->vertices = vertices;

    for (auto& vTuple : vertices)
    {
        printf("Vertex id: %d ... y: %f ... BoneID: %d ... poids: %f...\n", std::get<1>(vTuple).id, std::get<1>(vTuple).position.y, std::get<1>(vTuple).bonesID.x, std::get<0>(vTuple));
    }
}

Bone::~Bone()
{

}

void Bone::interpolateTransform(double animationTime, glm::mat4* bonesTransform, glm::mat4& parentTransforms)
{
    glm::mat4 res = parentTransforms;
    //const std::map<double, KeyFrame> keyFrames = keyFrames[animation];
    //printf("KeyFrame SIZE: %d\n", keyFrames.size());

    // Si aucune keyframe n'est disponible, retourner une matrice identité
    if (keyFrames.empty()) 
    {
        bonesTransform[id] = res * offetMatrix;
        for (Bone* child : children)
            child->interpolateTransform(animationTime, bonesTransform, res);
        return;
    }

    // Trouver les keyframes qui entourent le temps d'animation donné
    auto nextKeyFrame = keyFrames.lower_bound(animationTime);

    if (nextKeyFrame == keyFrames.end()) // Si aucune keyframe suivante n'est trouvée, utiliser la dernière keyframe
    {
        res *= glm::translate(keyFrames.rbegin()->second.position) * glm::mat4_cast(keyFrames.rbegin()->second.rotation) * glm::scale(keyFrames.rbegin()->second.scale);
        bonesTransform[id] = res * offetMatrix;
        for(Bone* child : children)
            child->interpolateTransform(animationTime, bonesTransform, res);
        return;
    }

    if (nextKeyFrame == keyFrames.begin()) // Si aucune keyframe précédente n'est trouvée, utiliser la première keyframe
    {
        res *= glm::translate(keyFrames.begin()->second.position) * glm::mat4_cast(keyFrames.begin()->second.rotation) * glm::scale(keyFrames.begin()->second.scale);
        for (Bone* child : children)
            child->interpolateTransform(animationTime, bonesTransform, res);
        return;
    }

    auto prevKeyFrame = std::prev(nextKeyFrame);

    //printf("AnimTime: %f ... KeyFramesSize: %d ... prev: %f ... next: %f\n", animationTime, keyFrames.size(), prevKeyFrame->first, nextKeyFrame->first);

    // Calculer le facteur d'interpolation
    float factor = static_cast<float>((animationTime - prevKeyFrame->first) / (nextKeyFrame->first - prevKeyFrame->first));
    // Interpoler les positions, rotations et échelles
    glm::vec3 position = glm::mix(prevKeyFrame->second.position, nextKeyFrame->second.position, factor);
    glm::quat rotation = glm::slerp(prevKeyFrame->second.rotation, nextKeyFrame->second.rotation, factor);
    glm::vec3 scale    = interpolate(prevKeyFrame->second.scale, nextKeyFrame->second.scale, factor);

    res *= glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
    bonesTransform[id] = res * offetMatrix;
    for (Bone* child : children)
        child->interpolateTransform(animationTime, bonesTransform, res);
    return;
}

void Bone::applyTransformations(glm::mat4 localTransform, glm::mat4 parentTransform, double animationTime)
{
    glm::mat4 globalTransform = localTransform * parentTransform;

    for (auto vertexTuple : vertices)
    {
        float  weight = std::get<0>(vertexTuple);
        Vertex vertex = std::get<1>(vertexTuple);

        if (weight > 0)
        {
            // Appliquer la transformation à la position avec le poids
            glm::vec4 transformedPosition = globalTransform * glm::vec4(vertex.position, 1.0f);
            vertex.position = glm::vec3(glm::mix(glm::vec4(vertex.position, 1.0f), transformedPosition, weight));

            // Appliquer la transformation à la normale avec le poids (sans translation)
            glm::vec4 transformedNormal = globalTransform * glm::vec4(vertex.normal, 0.0f);
            vertex.normal = glm::normalize(glm::mix(glm::vec4(vertex.normal, 0.0f), transformedNormal, weight));
        }
    }

    for (Bone* child : children)
    {
        if (child)
        {
            //child->applyTransformations(child->interpolateTransform(animationTime), globalTransform, animationTime);
        }
    }
}
