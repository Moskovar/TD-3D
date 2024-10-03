#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine(Game::Map* world, std::vector<Element*>* elements)
{
	this->world     = world;
    this->elements  = elements;
}

PhysicsEngine::~PhysicsEngine()
{
	if (world)
	{
		delete world;
		world = nullptr;
	}
}

GLfloat PhysicsEngine::distanceBetweenHitboxes(Element* e1, Element* e2)
{
    GLfloat distanceX = 0.0f;
    GLfloat distanceY = 0.0f;
    GLfloat distanceZ = 0.0f;

    glm::vec3 e1_maxPoint = e1->getRHitbox().maxPoint;
    glm::vec3 e1_minPoint = e1->getRHitbox().minPoint;
    glm::vec3 e2_maxPoint = e2->getRHitbox().maxPoint;
    glm::vec3 e2_minPoint = e2->getRHitbox().minPoint;

    // Axe X
    if (e1_maxPoint.x < e2_minPoint.x)
    {
        distanceX = e2_minPoint.x - e1_maxPoint.x;  // À gauche
    }
    else if (e1_minPoint.x > e2_maxPoint.x)
    {
        distanceX = e1_minPoint.x - e2_maxPoint.x;  // À droite
    }

    // Axe Y
    if (e1_maxPoint.y < e2_minPoint.y)
    {
        distanceY = e2_minPoint.y - e1_maxPoint.y;  // En-dessous
    }
    else if (e1_minPoint.y > e2_maxPoint.y)
    {
        distanceY = e1_minPoint.y - e2_maxPoint.y;  // Au-dessus
    }

    // Axe Z
    if (e1_maxPoint.z < e2_minPoint.z)
    {
        distanceZ = e2_minPoint.z - e1_maxPoint.z;  // Derrière
    }
    else if (e1_minPoint.z > e2_maxPoint.z)
    {
        distanceZ = e1_minPoint.z - e2_maxPoint.z;  // Devant
    }

    // Si les hitboxes se chevauchent, retourner 0
    if (distanceX <= 0.0f && distanceY <= 0.0f && distanceZ <= 0.0f)
    {
        return 0.0f;  // Collision
    }

    // Calculer la distance totale entre les deux hitboxes en 3D
    return glm::sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
}

bool PhysicsEngine::checkHeightMap(Element* element, glm::vec3 nextElementPos)//revoir les points comptés dans l'interpolation
{
    int chunkLength = (LARGETILE_SIZE * CHUNK_ARR_SIZE);

    Chunk* chunk = world->getChunk((int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);

    LargeTile* lt = chunk->getLargeTile(((int)element->getPosition().z % chunkLength) / LARGETILE_SIZE, ((int)element->getPosition().x % chunkLength) / LARGETILE_SIZE);

    nextElementPos.x = (int)nextElementPos.x % LARGETILE_SIZE;
    nextElementPos.z = (int)nextElementPos.z % LARGETILE_SIZE;

    int nextZ = (nextElementPos.z < LARGETILE_SIZE - 1) ? nextElementPos.z + 1 : nextElementPos.z - 1,
        nextX = (nextElementPos.x < LARGETILE_SIZE - 1) ? nextElementPos.x + 1 : nextElementPos.x - 1;

    GLfloat interpolatedY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

    y1 = lt->getVertex(nextZ, nextElementPos.x).y;                  y2 = lt->getVertex(nextZ, nextX).y;
    y3 = lt->getVertex(nextElementPos.z, nextElementPos.x).y;       y4 = lt->getVertex(nextElementPos.z, nextX).y;

    interpolatedY = (y1 + y2 + y3 + y4) / 4.0f;

    if (interpolatedY - nextElementPos.y > 2) return false;

    if (nextElementPos.y < interpolatedY) element->translate(glm::vec3(0.0f, element->getPosition().y - interpolatedY, 0.0f));

    return true;
}

void PhysicsEngine::applyGravity(Element* element, GLfloat deltaTime)
{
    if (static_cast<Entity*>(element) && static_cast<Entity*>(element)->isJumping()) return;

    glm::vec3* elementPos = element->getPositionP();

    int z = (int)elementPos->z % LARGETILE_SIZE,
        x = (int)elementPos->x % LARGETILE_SIZE;

    int chunkLength = LARGETILE_SIZE * CHUNK_ARR_SIZE,
        largeTileLength = TILE_SIZE * LARGETILE_ARR_SIZE;

    Chunk*      chunk   = world->getChunk((int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);
    LargeTile*  lt      = chunk->getLargeTile(((int)element->getPosition().z / largeTileLength) % CHUNK_ARR_SIZE, ((int)element->getPosition().x / largeTileLength) % CHUNK_ARR_SIZE);

    GLfloat yGround = lt->getVertex(z, x).y;;

    bool collision = false;
    GLfloat distance = 0.0f;

    if (elementPos->y > yGround) //Si l'élément n'est pas au sol
    {
        //std::cout << "y > yGround : elements size -> " << elements->size() << std::endl;
        for (Element* e : *elements)
        {
            if (checkCollision(element->getAnticipatedFallHitbox(deltaTime), e->getRHitbox()))
            {
                collision = true;

                GLfloat temp = distanceBetweenHitboxes(element, e);
                if (distance < temp) distance = temp;
                break;
            }
        }

        if (!collision)//s'il n'y a pas collision avec un elément alors on le fait tomber vers le sol
        {
            element->fall(deltaTime);
            element->setFall(true);
        }
        else if (collision && element->isFalling())
        {
            std::cout << "OH FALL COLLISION AT: " << distance << std::endl;
            element->translate(glm::vec3(0.0f, -distance + 0.001, 0.0f));
            element->setFall(false);
        }
    }

    //Si l'élément est tomber plus bas que le sol, on le remet au niveau du sol et on lui enlève son état de chute
    if (elementPos->y < yGround)
    {
        elementPos->y = yGround;
        element->setFall(false);
    }
}

bool PhysicsEngine::checkCollision(const OBB& obb1, const OBB& obb2) {
    // Les axes locaux des deux OBB (trois pour chaque)
    glm::vec3 axes1[3] = { obb1.orientation[0], obb1.orientation[1], obb1.orientation[2] };
    glm::vec3 axes2[3] = { obb2.orientation[0], obb2.orientation[1], obb2.orientation[2] };

    // Vecteur entre les centres des deux OBB
    glm::vec3 centerVector = obb2.center - obb1.center;

    // Tester les 15 axes potentiels de séparation (axes locaux + produits croisés)
    for (int i = 0; i < 3; ++i) {
        // Tester les axes locaux de obb1
        if (!testAxis(axes1[i], obb1, obb2, centerVector)) return false;

        // Tester les axes locaux de obb2
        if (!testAxis(axes2[i], obb1, obb2, centerVector)) return false;

        // Tester les produits croisés entre les axes locaux de obb1 et obb2
        for (int j = 0; j < 3; ++j) {
            glm::vec3 crossAxis = glm::cross(axes1[i], axes2[j]);
            if (glm::length(crossAxis) > 0.0001f) 
            {  // Tolérance pour ignorer les vecteurs nuls
                if (!testAxis(crossAxis, obb1, obb2, centerVector)) return false;
            }
        }
    }

    // Si aucune séparation n'a été trouvée, il y a collision
    return true;
}

bool PhysicsEngine::testAxis(const glm::vec3& axis, const OBB& obb1, const OBB& obb2, const glm::vec3& centerVector)
{
    // Projeter la distance entre les centres sur cet axe
    float distanceCenters = fabs(glm::dot(centerVector, axis));

    // Projeter la taille des OBB sur cet axe
    float projection1 = obb1.halfSize.x * fabs(glm::dot(obb1.orientation[0], axis)) +
        obb1.halfSize.y * fabs(glm::dot(obb1.orientation[1], axis)) +
        obb1.halfSize.z * fabs(glm::dot(obb1.orientation[2], axis));

    float projection2 = obb2.halfSize.x * fabs(glm::dot(obb2.orientation[0], axis)) +
        obb2.halfSize.y * fabs(glm::dot(obb2.orientation[1], axis)) +
        obb2.halfSize.z * fabs(glm::dot(obb2.orientation[2], axis));

    // Vérifier s'il y a séparation
    const float epsilon = 0.0001f;  // Tolérance numérique
    return distanceCenters <= (projection1 + projection2 + epsilon);
}
