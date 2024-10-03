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
PhysicsEngine*  physics;

std::map<char, bool> keyPressed;
std::map<int , bool> mousePressed;

Character* player = nullptr;

std::vector<Entity*>  entities;
std::vector<Element*> elements;

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
            player->setSelected();

        
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
    player = new Character(0, glm::vec3(300, 120.0f, 300));

    if(player) entities.push_back(player);
    else
    {
        printf("ERROR: PLAYER ENTITY NOT LOADED\n");
        return 1;
    }

    elements.push_back(new Element(0, glm::vec3(300, 100.0f, 300.0f), "models/obj/foundation.obj"));

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
    world   = new Game::Map(shaders);
    physics = new PhysicsEngine(world, &elements);
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

        //applyGravity(player, deltaTime);
        physics->applyGravity(player, deltaTime);

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

            if (physics->checkHeightMap(player, player->getAnticipatedMove(deltaTime * factor)[3]))//check si la map ne bloque pas
            {
                bool collision = false;
                GLfloat distance = 0.0f;
                for (Element* e : elements)
                {
                    if (physics->checkCollision(player->getAnticipatedMoveHitbox(deltaTime * factor), e->getRHitbox()))
                    {
                        GLfloat temp = physics->distanceBetweenHitboxes(player, e);
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
