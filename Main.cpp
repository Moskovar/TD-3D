#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Player.h"
#include "Entity.h"
#include "Nexus.h"
#include "Tower.h"
#include "Character.h"
#include "Camera.h"
#include "Shader.h"

#include "MapManager.h"
#include "PhysicsEngine.h"
#include "Map.h"
#include "Cell.h"

#include "stb_image.h"

#include <tinyexr.h>

GLfloat width = 1920.0f, height = 1080.0f;

Window*         window  = nullptr;
Camera*         camera  = nullptr;
Game::Map*      world   = nullptr;
Player*         rPlayer = nullptr;
Player*         lPlayer = nullptr;
glm::mat4* view         = nullptr;
glm::mat4 projection;
MapManager      mapManager;
PhysicsEngine*  physics;

std::map<char, bool> keyPressed;
std::map<int , bool> mousePressed;

std::vector<Cell> cells;

//Character* player = nullptr;

std::vector<Entity*>    entities;
std::vector<Element*>   elements;
std::vector<Tower*>     towers;

glm::vec3 generateRayFromCursor(GLFWwindow* window)
{
    // 1. Récupérer la position du curseur
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    //std::cout << xpos << " : " << ypos << std::endl;

    // 2. Convertir en coordonnées NDC
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float xNDC = (2.0f * xpos) / width - 1.0f;
    float yNDC = 1.0f - (2.0f * ypos) / height;  // Inverser Y

    // 3. Passer à l'espace vue
    glm::vec4 clipCoords = glm::vec4(xNDC, yNDC, -1.0f, 1.0f);
    glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    // 4. Passer à l'espace monde
    glm::vec4 worldCoords = glm::inverse(*view) * eyeCoords;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));

    // Maintenant rayDirection est la direction dans le monde vers laquelle le curseur pointe.
    //std::cout << "RayDirection: " << rayDirection.x << " : " << rayDirection.y << " : " << rayDirection.z << std::endl;
    return rayDirection;
}

bool findRayIntersectionWithMap(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& worldPos) {
    // Vérifie que le rayon n'est pas parallèle au plan
    const float epsilon = 0.0001f;
    if (fabs(rayDirection.y) < epsilon || rayDirection.y >= 0.0f) {
        return false;
    }

    // Calcul de t
    float t = -rayOrigin.y / rayDirection.y;

    // Calcul du point d'intersection
    worldPos = rayOrigin + t * rayDirection;
    return true;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if      (action == GLFW_PRESS)      mousePressed[button] = true;
    else if (action == GLFW_RELEASE) 
    {
        mousePressed[button] = false;
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            glm::vec3 worldPos;
            if (findRayIntersectionWithMap(camera->getPosition(), generateRayFromCursor(window), worldPos));

            bool isCellTargeted = false;
            for (Cell& cell : cells)
            {
                if ((int)worldPos.x / 8 == (int)cell.getX() / 8 && (int)worldPos.z / 8 == (int)cell.getZ() / 8)
                {
                    isCellTargeted = true;
                    std::cout << "IN CELL!!" << std::endl;
                    if(cell.isBuildable())
                    {
                        std::cout << "BUIDL !!" << std::endl;
                        Tower* tower = new Tower(0, glm::vec3(((int)worldPos.x / 8) * 8 + 4, 0.0f, ((int)worldPos.z / 8) * 8 + 4), "models/fbx/doublecube.fbx");
                        towers.push_back(tower);
                        cell.setTower(tower);
                    }
                    break;
                }
            }
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "KeyPressed callback" << std::endl;
    if (action == GLFW_PRESS) 
    {
        keyPressed[key] = true;

        switch (key) 
        {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);   break;
            case GLFW_KEY_Q: entities.push_back(new Character(0, glm::vec3(getCellCenter(1052), 0.0f, getCellCenter(930)))); break;
        }      
    }
    else if (action == GLFW_RELEASE)
    {
        keyPressed[key] = false;
    }
}

void processKeyPressed(GLFWwindow* window, float deltaTime)
{
    //if(keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_S])
    //{
    //    player->setMove(true);
    //}

    //if (keyPressed[GLFW_KEY_D]) 
    //{
    //    player->turn(TURN_SPEED   * deltaTime);
    //    camera->addYaw(TURN_SPEED * deltaTime);
    //}
    //if (keyPressed[GLFW_KEY_A]) 
    //{
    //    player->turn(TURN_SPEED   * -deltaTime);
    //    camera->addYaw(TURN_SPEED * -deltaTime);
    //}

    //if (keyPressed[GLFW_KEY_SPACE] && !player->isFalling())
    //{
    //    player->setJumping(true);
    //}
}

void processMousePressed(Window* window, float deltaTime)
{
    GLFWwindow* glfwWindow = window->getGLFWWindow();

    //if (mousePressed[GLFW_MOUSE_BUTTON_LEFT])
    //{

    //}
}

bool rayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& outIntersection) {
    const float EPSILON = 0.0000001;
    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = glm::cross(rayDir, edge2);
    a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;    // Le rayon est parallèle au triangle

    f = 1.0 / a;
    s = rayOrigin - v0;
    u = f * glm::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    q = glm::cross(s, edge1);
    v = f * glm::dot(rayDir, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // Calculer la distance de l'intersection
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) // Si t > EPSILON, il y a une intersection
    {
        outIntersection = rayOrigin + rayDir * t;
        return true;
    }

    return false;
}

void checkWorldInteractions()
{
    GLFWwindow* glfwWindow = window->getGLFWWindow();
    LargeTile* lt = world->getChunk(0, 0)->getLargeTile(1, 1);
    
    std::vector<HeightMapVertex> v_vertices = lt->getTile(15, 15)->getVertices();
    std::vector<unsigned int>	 v_indices  = lt->getTile(15, 15)->getIndices();

    for (unsigned int i = 0; i < v_indices.size() - 2; i += 3)
    {
        glm::vec3   v_pos1 = glm::vec3(v_vertices[v_indices[i    ]].x, v_vertices[v_indices[i    ]].y, v_vertices[v_indices[i    ]].z),
                    v_pos2 = glm::vec3(v_vertices[v_indices[i + 1]].x, v_vertices[v_indices[i + 1]].y, v_vertices[v_indices[i + 1]].z),
                    v_pos3 = glm::vec3(v_vertices[v_indices[i + 2]].x, v_vertices[v_indices[i + 2]].y, v_vertices[v_indices[i + 2]].z),
                    intersection;
        std::cout << rayIntersectsTriangle(camera->getPosition(), generateRayFromCursor(glfwWindow), v_pos1, v_pos2, v_pos3, intersection) << std::endl;
    }
}

void manageEntities()
{
    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [](Entity* entity) 
            {
                if (entity && !entity->isAlive())
                {
                    delete entity;
                    entity = nullptr;
                }
                    return entity == nullptr; 
            }),
        entities.end()
    );
    //std::cout << entities.size() << std::endl;
}


int main()
{
    //--- Chargement du contexte OpenGL ---//
    window = new Window(width, height);
    window->fullScreen();
    window->keepCursorInWindow();
    GLFWwindow* glfwWindow = window->getGLFWWindow();

    glfwSetWindowPos(glfwWindow, 1920, 0);
    //glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);  // Clics de souris

    rPlayer = new Player(true);
    lPlayer = new Player(false);

    GLfloat yaw = -90.0f;
    glm::vec3 target = glm::vec3(1024.0f, 0.0f, 1024.0f);

    //--- Chargement de la caméra ---//
    camera = new Camera(&target, &yaw, &keyPressed);

    view        = camera->getViewMatrixP();
    projection  = glm::perspective(glm::radians(45.0f), width / height, 0.5f, 2000.0f);

    //--- Chargement des shaders ---//
    std::map<std::string, Shader> shaders;
    shaders["AnimatedObject"]           = Shader("shaders/vertex_shader.glsl"        , "shaders/fragment_shader.glsl"                  , view, &projection);
    shaders[MAP_JUNCTIONS_SHADERS]      = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/map_fragment_shader.glsl"          , view, &projection);
    shaders[CHUNKS_JUNCTIONS_SHADERS]   = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/chunks_fragment_shader.glsl"       , view, &projection);
    shaders[LARGETILES_SHADERS]         = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/largetiles_fragment_shader.glsl"   , view, &projection);

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //if (glIsEnabled(GL_DEPTH_TEST)) std::cout << "Depth test is enabled."     << std::endl;
    //else                            std::cout << "Depth test is not enabled." << std::endl;

    //--- Chargement de la map ---//
    world   = new Game::Map(shaders);
    physics = new PhysicsEngine(world, &elements);
    mapManager.loadMap(world, shaders);

    for (GLfloat z = 960; z < 1088; z += 8)
    {
        for(int x = 0; x < 3; ++x)
        {
            cells.push_back(Cell(1024.0f + x       * 8, z));
            cells.push_back(Cell(1024.0f - (x + 1) * 8, z));//x + 1 sinon on a 2 rangé sur le même x
        }

        for (int x = 4; x < 6; ++x)
        {
            cells.push_back(Cell(1024.0f +  x      * 8, z));
            cells.push_back(Cell(1024.0f - (x + 1) * 8, z));//x + 1 sinon on a 2 rangé sur le même x
        }
    }

    auto  startTime    = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame    = glfwGetTime(), deltaTime = 0, now = 0;
    GLboolean run = true;

    Texture textureTemp("textures/basic_texture_1.png");//??
    
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow) && run)
    {   //AnimationTime
        auto currentTime = std::chrono::high_resolution_clock::now();
        timeSinceStart = std::chrono::duration<float>(currentTime - startTime).count();

        //DeltaTime
        now       = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        manageEntities();
        
        //applyGravity(player, deltaTime);
        //physics->applyGravity(player, deltaTime);
        //checkWorldInteractions();
        
        glm::vec3 worldPos;
        findRayIntersectionWithMap(camera->getPosition(), generateRayFromCursor(glfwWindow), worldPos);
        //std::cout << worldPos.x << " : " << worldPos.z << std::endl;

        bool isCellTargeted = false;
        for (Cell& cell : cells)
        {
            if ((int)worldPos.x / 8 == (int)cell.getX() / 8 && (int)worldPos.z / 8 == (int)cell.getZ() / 8)
            {
                isCellTargeted = true;
                break;
            }
        }

        shaders[LARGETILES_SHADERS].use();
        glUniform1i(glGetUniformLocation(shaders[LARGETILES_SHADERS].getShaderProgram(), "isCellTargeted"), isCellTargeted ? 1 : 0); // 1 si true, 0 si false
        glUniform3f(glGetUniformLocation(shaders[LARGETILES_SHADERS].getShaderProgram(), "worldPos"), worldPos.x, worldPos.y, worldPos.z);

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getMouseX(), window->getMouseY(), window->getScrollValue(), deltaTime);

        //processKeyPressed(glfwWindow, deltaTime);
        processMousePressed(window, deltaTime);

        // Effacer le buffer de couleur et de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textureTemp.useTexture();//??

        // Utiliser le programme de shaders
        shaders["AnimatedObject"].use();

        //--- Joueur ---//
        rPlayer->nexusSpawn(timeSinceStart);
        lPlayer->nexusSpawn(timeSinceStart);

        rPlayer->render(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart, deltaTime);
        lPlayer->render(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart, deltaTime);

        auto endTime    = std::chrono::high_resolution_clock::now();
        auto duration   = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        //--- Render objects ---//
        for (Tower* t : towers)
        {
            t->attack(entities, duration.count());
            t->render(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart);
        }


        //--- Render terrain ---//
        world->render();

        //--- Reset des mouvements souris dans la fenêtre pour traiter les prochains ---//
        window->resetXYChange();

        //Swap buffers
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    run = false;

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
