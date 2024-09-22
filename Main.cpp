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

std::vector<Entity*>  entities;
std::vector<Element*> elements;

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
    if (static_cast<Entity*>(element) && static_cast<Entity*>(element)->isJumping()) return;

    glm::vec3* elementPos = element->getPositionP();

    int z = (int)elementPos->z % LARGETILE_SIZE,
        x = (int)elementPos->x % LARGETILE_SIZE;

    int chunkLength     = LARGETILE_SIZE * CHUNK_ARR_SIZE,
        largeTileLength = TILE_SIZE * LARGETILE_ARR_SIZE;

    Chunk*     chunk = world->getChunk(    (int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);
    LargeTile* lt    = chunk->getLargeTile(((int)element->getPosition().z / largeTileLength) % CHUNK_ARR_SIZE, ((int)element->getPosition().x / largeTileLength) % CHUNK_ARR_SIZE);

    //std::cout << "CHUNK: " << (int)element->getPosition().z / chunkLength << " ... " << (int)element->getPosition().x / chunkLength << std::endl;
    //std::cout << "LT: " << (int)element->getPosition().z / largeTileLength << " ... " << (int)element->getPosition().x / largeTileLength << std::endl;

    int nextZ = (z < LARGETILE_SIZE - 1) ? z + 1 : z - 1,
        nextX = (x < LARGETILE_SIZE - 1) ? x + 1 : x - 1;

    //std::cout << "NextZ: " << nextZ << " ... " << "NextX: " << nextX << std::endl;

    GLfloat interpolatedGroundY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

    //y1 = lt->getVertex(nextZ, x).y;      y2 = lt->getVertex(nextZ, nextX).y;
    //y3 = lt->getVertex(z    , x).y;      y4 = lt->getVertex(z    , nextX).y;

    //interpolatedGroundY = (y1 + y2 + y3 + y4) / 4.0f;
    interpolatedGroundY = lt->getVertex(z, x).y;

    //std::cout << elementPos->y << " ... " << interpolatedGroundY << std::endl;
    if(elementPos->y > interpolatedGroundY) //Si l'élément n'est pas au sol on le fait tomber
    {
        element->fall(deltaTime);
        element->setFall(true);
    }

    //Si l'élément est tomber plus bas (ou même niveau) que le sol, on le remet au niveau du sol et on lui enlève son état de chute
    if (elementPos->y <= interpolatedGroundY)
    {
        elementPos->y = interpolatedGroundY;
        element->setFall(false);
    }
}

bool checkCollision(const AABB& box1, const AABB& box2) 
{
    //std::cout << box1.max_point.x << " < " << box2.min_point.x << " || " << box1.min_point.x << " > " << box2.max_point.x << std::endl;
    // Vérifier l'axe x
    if (box1.max_point.x < box2.min_point.x || box1.min_point.x > box2.max_point.x)
        return false; // Pas de collision sur l'axe x

    //std::cout << box1.max_point.y << " < " << box2.min_point.y << " || " << box1.min_point.y << " > " << box2.max_point.y << std::endl;
    // Vérifier l'axe y
    if (box1.max_point.y < box2.min_point.y || box1.min_point.y > box2.max_point.y)
        return false; // Pas de collision sur l'axe y

    //std::cout << box1.max_point.z << " < " << box2.min_point.z << " || " << box1.min_point.z << " > " << box2.max_point.z << std::endl;
    //// Vérifier l'axe z
    if (box1.max_point.z < box2.min_point.z || box1.min_point.z > box2.max_point.z)
        return false; // Pas de collision sur l'axe z

    // Les boîtes se chevauchent
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

        if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT && !keyPressed['W'] && !keyPressed['S']) entities[0]->setMove(false);

        if (static_cast<Character*>(entities[0])->getSelectedBuilding() && button == GLFW_MOUSE_BUTTON_LEFT)
        {
            elements.push_back(new Element(0, static_cast<Character*>(entities[0])->getSelectedBuilding()->getPosition(), "models/obj/foundation.obj"));
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
            entities[0]->directionPressed(key);
        }

        switch (key) 
        {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);   break;
        }

        if (key == GLFW_KEY_Q)
        {
            static_cast<Character*>(entities[0])->setSelected();
            std::cout << "A PRESSED !! " << std::endl;
        }

        
    }
    else if (action == GLFW_RELEASE)
    {
        keyPressed[key] = false;

        if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A)
        {
            entities[0]->directionReleased(key);
            if(!keyPressed[GLFW_KEY_W] && !keyPressed[GLFW_KEY_S])//Si ni avancer ni reculer n'est pressé, move = false;
                entities[0]->setMove(false);
        }

        if (key == GLFW_KEY_Q) static_cast<Character*>(entities[0])->clearSelected();

        std::cout << "A RELEASED !! " << std::endl;
    }
}

void processKeyPressed(GLFWwindow* window, float deltaTime)
{
    if(keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_S])
    {
        entities[0]->setMove(true);
    }

    if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (keyPressed[GLFW_KEY_D]) entities[0]->turn(TURN_SPEED * deltaTime);
        if (keyPressed[GLFW_KEY_A]) entities[0]->turn(TURN_SPEED * -deltaTime);
    }

    if (keyPressed[GLFW_KEY_SPACE] && !entities[0]->isFalling())
    {
        entities[0]->setJumping(true);
    }
}

void processMousePressed(Window* window, float deltaTime)
{
    GLFWwindow* glfwWindow = window->getGLFWWindow();

    if (mousePressed[GLFW_MOUSE_BUTTON_RIGHT])
    {
        if (mousePressed[GLFW_MOUSE_BUTTON_LEFT])
        {
            GLfloat offsetYaw = camera->getYaw() - entities[0]->getYaw();
            //std::cout << offsetYaw << std::endl;
            entities[0]->turn(offsetYaw);

            entities[0]->setMove(true);
        }

        if (window->getXChange() > 0) entities[0]->turn(camera->getSensitivity() * deltaTime);
        else if (window->getXChange() < 0) entities[0]->turn(camera->getSensitivity() * -deltaTime);
    }
}


int main()
{
    window = new Window(800, 600);
    glfwSetWindowPos(window->getGLFWWindow(), 2100, 200);

    entities.push_back(new Character(0, glm::vec3(300, 100.0f, 300)));
    entities.push_back(new Entity(1, glm::vec3(300.0f, 100.0f, 300.0f), "models/fbx/doublecube.fbx"));

    elements.push_back(new Element(0, glm::vec3(310, 100.0f, 300.0f), "models/obj/foundation.obj"));

    //entities.push_back(new Entity(2, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/ground.fbx"));

    camera = new Camera(entities[0]->getPositionP(), entities[0]->getPYaw(), &keyPressed);

    glm::mat4* view = camera->getViewMatrixP();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 500.0f);

    //--- Création des shaders ---//
    std::map<std::string, Shader> shaders;
    shaders["AnimatedObject"]                = Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"                          , view, &projection);
    shaders[MAP_JUNCTIONS_SHADERS]           = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/map_fragment_shader.glsl"          , view, &projection);
    shaders[CHUNKS_JUNCTIONS_SHADERS]        = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/chunks_fragment_shader.glsl"       , view, &projection);
    shaders[LARGETILES_SHADERS]              = Shader("shaders/map/map_vertex_shader.glsl", "shaders/map/largetiles_fragment_shader.glsl"   , view, &projection);

    GLFWwindow* glfwWindow = window->getGLFWWindow();
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);  // Clics de souris

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glIsEnabled(GL_DEPTH_TEST)) std::cout << "Depth test is enabled."     << std::endl;
    else                            std::cout << "Depth test is not enabled." << std::endl;

    world = new Game::Map(shaders);
    mapManager.loadMap(world, shaders);

    auto  startTime    = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame    = glfwGetTime(), deltaTime = 0, now = 0;

    Texture textureTemp("textures/basic_texture_1.png");

    GLint maxTextures;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
    std::cout << "Max textures supported: " << maxTextures << std::endl;

    std::cout << "VERTEX Y: " << world->getChunk(0, 0)->getLargeTile(0, 0)->getVertex(511, 511).y << std::endl;;
    
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow))
    {   //AnimationTime
        auto currentTime = std::chrono::high_resolution_clock::now();
        timeSinceStart = std::chrono::duration<float>(currentTime - startTime).count();

        //DeltaTime
        now       = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        applyGravity(entities[0], deltaTime);

        //--- Personnage ---//
        processKeyPressed(glfwWindow, deltaTime);
        processMousePressed(window, deltaTime);

        if(entities[0]->isJumping()) entities[0]->jump(deltaTime);

        if(entities[0]->isMoving())
        {
            GLfloat factor = 1;

            if (keyPressed['S'] && (!mousePressed[GLFW_MOUSE_BUTTON_LEFT] || !mousePressed[GLFW_MOUSE_BUTTON_RIGHT]) && !keyPressed['W'])//Si on recule
            {
                factor = -0.5;//Vitesse divisée par 2 en reculant
            }
            else if (keyPressed['W'] && keyPressed['S']) factor = 0;//si Z et S sont appuyés, on bouge pas

            if (entities[0]->isFalling()) factor /= 2;
            
            if(checkHeightMap(entities[0], entities[0]->anticipateMove(deltaTime * factor)))//check si la map ne bloque pas
            {
                bool collision = false;

                for(Element* e : elements)
                    if (checkCollision(entities[0]->getAnticipatedHitbox(deltaTime), e->getRHitbox()))
                    {
                        collision = true;
                        break;
                    }

                if(!collision) entities[0]->move(deltaTime * factor);
            }

            if (!mousePressed[GLFW_MOUSE_BUTTON_LEFT] && !mousePressed[GLFW_MOUSE_BUTTON_RIGHT]) camera->resetYaw();
        }

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue(), deltaTime);

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

        static_cast<Character*>(entities[0])->renderSelected(shaders["AnimatedObject"].modelLoc, shaders["AnimatedObject"].bonesTransformsLoc, timeSinceStart);

        //--- Render terrain ---//
        //world->getChunk(0, 0)->render();
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
