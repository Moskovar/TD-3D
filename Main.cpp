#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Character.h"
#include "Camera.h"
#include "Shader.h"

#include "MapManager.h"
#include "PhysicsEngine.h"
#include "Map.h"

#include "stb_image.h"

#include <tinyexr.h>

Window*         window  = nullptr;
Camera*         camera  = nullptr;
Game::Map*      world   = nullptr;
MapManager      mapManager;
PhysicsEngine   physics;

std::map<char, bool> keyPressed;
std::map<int , bool> mousePressed;

Character* player = nullptr;

std::vector<Entity*>  entities;
std::vector<Element*> elements;

GLfloat distanceBetweenHitboxes(Element* e1, Element* e2) 
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

glm::vec3 applyTransformation(const glm::mat4& transformMatrix, const glm::vec3& point) {
    // Convertir glm::vec3 en glm::vec4
    glm::vec4 point4D = glm::vec4(point, 1.0f); // Composante homogène = 1.0

    // Appliquer la matrice de transformation
    glm::vec4 transformedPoint = transformMatrix * point4D;

    // Retourner le point transformé en glm::vec3
    return glm::vec3(transformedPoint); // Ignore la composante homogène
}

bool checkHeightMap(Element* element, glm::vec3 nextElementPos)//revoir les points comptés dans l'interpolation
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

    if (nextElementPos.y < interpolatedY) element->moveUp(interpolatedY);

    return true;
}

void applyGravity(Element* element, GLfloat deltaTime)
{
    if (static_cast<Entity*>(element) && static_cast<Entity*>(element)->isJumping()) return;//?? why entity

    glm::vec3* elementPos = element->getPositionP();

    int z = (int)elementPos->z % LARGETILE_SIZE,
        x = (int)elementPos->x % LARGETILE_SIZE;

    int chunkLength     = LARGETILE_SIZE * CHUNK_ARR_SIZE,
        largeTileLength = TILE_SIZE * LARGETILE_ARR_SIZE;

    Chunk*     chunk = world->getChunk(    (int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);
    LargeTile* lt    = chunk->getLargeTile(((int)element->getPosition().z / largeTileLength) % CHUNK_ARR_SIZE, ((int)element->getPosition().x / largeTileLength) % CHUNK_ARR_SIZE);

    GLfloat yGround = lt->getVertex(z, x).y;;

    bool collision = false;
    GLfloat distance = 0.0f;

    if(elementPos->y > yGround) //Si l'élément n'est pas au sol
    {
        //for (Element* e : elements)
        //    if (checkCollision(player->getAnticipatedFallHitbox(deltaTime), e->getRHitbox2()))
        //    {
        //        collision = true;

        //        //GLfloat temp = distanceBetweenHitboxes(player->getRHitbox(), e->getRHitbox());
        //        //if (distance < temp) distance = temp;
        //        std::cout << "OH COLLISION!" << std::endl;
        //        element->setFall(false);
        //        break;
        //    }

        if(!collision)//s'il n'y a pas collision avec un elément alors on le fait tomber vers le sol
        {
            element->fall(deltaTime);
            element->setFall(true);
        }
        //else player->moveUp(-distance);
    }

    //Si l'élément est tomber plus bas que le sol, on le remet au niveau du sol et on lui enlève son état de chute
    if (elementPos->y < yGround)
    {
        elementPos->y = yGround;
        element->setFall(false);
    }
}

bool testAxis(const glm::vec3& axis, const OBB& obb1, const OBB& obb2, const glm::vec3& centerVector) {
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

bool checkCollision(const OBB& obb1, const OBB& obb2) {
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
            if (glm::length(crossAxis) > 0.0001f) {  // Tolérance pour ignorer les vecteurs nuls
                if (!testAxis(crossAxis, obb1, obb2, centerVector)) return false;
            }
        }
    }

    // Si aucune séparation n'a été trouvée, il y a collision
    return true;
}

bool checkCollision(Element* element1, Element* element2)
{
    return checkCollision(element1->getRHitbox(), element2->getRHitbox());
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if      (action == GLFW_PRESS)      mousePressed[button] = true;
    else if (action == GLFW_RELEASE) 
    {
        mousePressed[button] = false;

        if (player->getSelectedBuilding() && button == GLFW_MOUSE_BUTTON_LEFT)
        {
            Element* element = new Element(0, player->getSelectedBuilding()->getModelMtx(), "models/obj/foundation.obj");
            //element->setModelMtx(player->getSelectedBuilding()->getModelMtx());
            elements.push_back(element);
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "KeyPressed callback" << std::endl;
    if (action == GLFW_PRESS) 
    {
        keyPressed[key] = true;

        if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A)//sert à quoi ??
        {
            player->directionPressed(key);
        }

        switch (key) 
        {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);   break;
        }

        if (key == GLFW_KEY_Q)
        {
            player->setSelected();
            std::cout << "A PRESSED !! " << std::endl;
        }

        
    }
    else if (action == GLFW_RELEASE)
    {
        keyPressed[key] = false;

        if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A)
        {
            player->directionReleased(key);
            if(!keyPressed[GLFW_KEY_W] && !keyPressed[GLFW_KEY_S])//Si ni avancer ni reculer n'est pressé, move = false;
                player->setMove(false);
        }

        if (key == GLFW_KEY_Q) player->clearSelected();

        if (key == GLFW_KEY_E) elements[0]->turn(45.0f);
    }
}

void processKeyPressed(GLFWwindow* window, float deltaTime)
{
    if(keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_S])
    {
        player->setMove(true);
    }

    if (keyPressed[GLFW_KEY_D]) 
    {
        player->turn(TURN_SPEED   * deltaTime);
        camera->addYaw(TURN_SPEED * deltaTime);
    }
    if (keyPressed[GLFW_KEY_A]) 
    {
        player->turn(TURN_SPEED   * -deltaTime);
        camera->addYaw(TURN_SPEED * -deltaTime);
    }

    if (keyPressed[GLFW_KEY_SPACE] && !player->isFalling())
    {
        player->setJumping(true);
    }
}

void processMousePressed(Window* window, float deltaTime)
{
    GLFWwindow* glfwWindow = window->getGLFWWindow();

    //if (mousePressed[GLFW_MOUSE_BUTTON_RIGHT])
    //{
    //    if (mousePressed[GLFW_MOUSE_BUTTON_LEFT])
    //    {
    //        GLfloat offsetYaw = camera->getYaw() - player->getYaw();
    //        //std::cout << offsetYaw << std::endl;
    //        player->turn(offsetYaw);

    //        player->setMove(true);
    //    }

    //    if (window->getXChange() > 0) player->turn(camera->getSensitivity() * deltaTime);
    //    else if (window->getXChange() < 0) player->turn(camera->getSensitivity() * -deltaTime);
    //}
}

void processCameraMoved()
{
    GLfloat offsetYaw = camera->getYaw() - player->getYaw();
    if(offsetYaw != 0)  player->turn(offsetYaw);
}

int main()
{
    //--- Chargement du contexte OpenGL ---//
    window = new Window(800, 600);
    GLFWwindow* glfwWindow = window->getGLFWWindow();

    glfwSetWindowPos(glfwWindow, 2100, 200);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);  // Clics de souris

    //--- Chargement du joueur ---//
    player = new Character(0, glm::vec3(300, 100.0f, 300));

    if(player) entities.push_back(player);
    else
    {
        printf("ERROR: PLAYER ENTITY NOT LOADED\n");
        return 1;
    }

    elements.push_back(new Element(0, glm::vec3(310, 100.0f, 300.0f), "models/obj/foundation.obj"));

    //--- Chargement de la caméra ---//
    camera = new Camera(player->getPositionP(), player->getPYaw(), &keyPressed);

    glm::mat4* view         = camera->getViewMatrixP();
    glm::mat4 projection    = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 500.0f);

    //--- Chargement des shaders ---//
    std::map<std::string, Shader> shaders;
    shaders["AnimatedObject"]           = Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"                          , view, &projection);
    shaders[MAP_JUNCTIONS_SHADERS]      = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/map_fragment_shader.glsl"          , view, &projection);
    shaders[CHUNKS_JUNCTIONS_SHADERS]   = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/chunks_fragment_shader.glsl"       , view, &projection);
    shaders[LARGETILES_SHADERS]         = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/largetiles_fragment_shader.glsl"   , view, &projection);

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //if (glIsEnabled(GL_DEPTH_TEST)) std::cout << "Depth test is enabled."     << std::endl;
    //else                            std::cout << "Depth test is not enabled." << std::endl;

    //--- Chargement de la map ---//
    world = new Game::Map(shaders);
    mapManager.loadMap(world, shaders);

    auto  startTime    = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame    = glfwGetTime(), deltaTime = 0, now = 0;

    Texture textureTemp("textures/basic_texture_1.png");
    
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow))
    {   //AnimationTime
        auto currentTime = std::chrono::high_resolution_clock::now();
        timeSinceStart = std::chrono::duration<float>(currentTime - startTime).count();

        //DeltaTime
        now       = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        applyGravity(player, deltaTime);

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue(), deltaTime);

        //--- Personnage ---//
        //if (player->isJumping()) player->jump(deltaTime);

        //std::cout << "MOVE: " << player->isMoving() << std::endl;

        if (player->isMoving())
        {
            GLfloat factor = 1;

            if (keyPressed['S'] && !keyPressed['W'])//Si on recule
            {
                factor = -0.5;//Vitesse divisée par 2 en reculant
            }
            else if (keyPressed['W'] && keyPressed['S']) factor = 0;//si Z et S sont appuyés, on bouge pas

            //if (player->isFalling()) factor /= 2;

            if (checkHeightMap(player, player->getAnticipatedMove(deltaTime * factor)[3]))//check si la map ne bloque pas
            {
                bool collision = false;
                GLfloat distance = 0.0f;
                for (Element* e : elements)
                {
                    if (checkCollision(player->getAnticipatedMoveHitbox(deltaTime * factor), e->getRHitbox()))
                    {
                        GLfloat temp = distanceBetweenHitboxes(player, e);
                        //std::cout << "DIST: " << temp << std::endl;
                        if (distance < temp) distance = temp;
                        std::cout << "COLLISION AT: " << distance << std::endl;

                        collision = true;
                    }
                }
                //player->move(deltaTime * factor);
                if (!collision) player->move(deltaTime * factor);
                else            player->moveForward(distance);
            }
        }

        processCameraMoved();
        processKeyPressed(glfwWindow, deltaTime);
        processMousePressed(window, deltaTime);

        // Effacer le buffer de couleur et de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textureTemp.useTexture();//??

        // Utiliser le programme de shaders
        shaders["AnimatedObject"].use();
        
        for (Entity* e : entities)
            if(e) e->render(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart);

        //--- Render objects ---//
        for (Element* e : elements)
            e->render(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart);

        player->renderSelected(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart);

        //std::cout << player->getRHitbox().orientation[2].x << " : " << player->getRHitbox().orientation[2].y << " : " << player->getRHitbox().orientation[2].z << std::endl;
        //std::cout << elements[0]->getRHitbox().orientation[0].x << " : " << elements[0]->getRHitbox().orientation[0].y << " : " << elements[0]->getRHitbox().orientation[0].z << std::endl;
        //std::cout << elements[0]->getRHitbox().orientation[1].x << " : " << elements[0]->getRHitbox().orientation[1].y << " : " << elements[0]->getRHitbox().orientation[1].z << std::endl;
        //std::cout << elements[0]->getRHitbox().orientation[2].x << " : " << elements[0]->getRHitbox().orientation[2].y << " : " << elements[0]->getRHitbox().orientation[2].z << std::endl;

        //--- Render terrain ---//
        world->render();

        //--- Reset des mouvements souris dans la fenêtre pour traiter les prochains ---//
        window->resetXYChange();

        //Swap buffers
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    // Nettoyer et quitter
    if (window)
    {
        delete window;
        window = nullptr;
    }

    if (camera)
    {
        delete camera;
        camera = nullptr;
    }
    
    for(Entity* e : entities)
        if (e) 
        {
            delete e;
            e = nullptr;
        }

    for(Element* e : elements)
        if (e)
        {
            delete e;
            e = nullptr;
        }

    if (world)
    {
        delete world;
        world = nullptr;
    }

    return 0;
}
