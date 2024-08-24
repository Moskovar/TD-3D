#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"

#include "PhysicsEngine.h"
#include "LargeTile.h"

#include "stb_image.h"

#include <tinyexr.h>

Window* window = nullptr;
Camera* camera = nullptr;
LargeTile* largeTile = nullptr;
PhysicsEngine physics;
//float r = 0.5f, g = 0.5f, b = 0.5f;
std::map<char, bool> keyPressed;

std::vector<Entity*> entities;

void applyGravity(Element* element, GLfloat deltaTime)
{
    //for (int i = 1; i < entities.size(); i++)
    //    if (checkCollision(entities[0]->getRHitbox(), entities[i]->getRHitbox())) return;

    glm::vec3* elementPos = element->getPositionP();

    if (elementPos->x >= 0 && elementPos->x < 512 && elementPos->z >= 0 && elementPos->z < 512)//Si le joueur est dans la heightmap
    {
        GLfloat interpolatedGroundY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

        y1 = largeTile->getVertex(elementPos->z + 1, elementPos->x).y;      y2 = largeTile->getVertex(elementPos->z + 1, elementPos->x + 1).y;
        y3 = largeTile->getVertex(elementPos->z    , elementPos->x).y;      y4 = largeTile->getVertex(elementPos->z    , elementPos->x + 1).y;

        interpolatedGroundY = (y1 + y2 + y3 + y4) / 4.0f;

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
}

bool checkHeightMap(glm::vec3 playerPos)//revoir les points comptés dans l'interpolation
{
    if (playerPos.x >= 0 && playerPos.x < 512 && playerPos.z >= 0 && playerPos.z < 512)//Si le joueur est dans la heightmap
    {
        

        GLfloat interpolatedY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

        y1 = largeTile->getVertex(playerPos.z + 1, playerPos.x).y;      y2 = largeTile->getVertex(playerPos.z + 1, playerPos.x + 1).y;
        y3 = largeTile->getVertex(playerPos.z    , playerPos.x).y;      y4 = largeTile->getVertex(playerPos.z    , playerPos.x + 1).y;
        //std::cout << y1 << std::endl;

        interpolatedY = (y1 + y2 + y3 + y4) / 4.0f;
        std::cout << interpolatedY - playerPos.y << std::endl;
        if (interpolatedY - playerPos.y > 2) return false;

        if(playerPos.y < interpolatedY) entities[0]->moveUp(interpolatedY);

        return true;
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
    }
    else if (action == GLFW_RELEASE)
    {
        keyPressed[key] = false;

        if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_A)
        {
            entities[0]->directionReleased(key);
            entities[0]->setMove(false);
        }
    }
}

void processKeyPressed(GLFWwindow* window, float deltaTime)
{
    bool collision = false;

    //for (int i = 1; i < entities.size(); i++)
    //    if ((collision = checkCollision(entities[0]->getRHitbox(), entities[i]->getRHitbox()))) return;


    if(keyPressed[GLFW_KEY_W])
    {
        
        if (checkHeightMap(entities[0]->anticipateMove(deltaTime)))
        {
            entities[0]->move(deltaTime);
            if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) camera->resetYaw();
        }
    }

    if (keyPressed[GLFW_KEY_S])
    {
        if(checkHeightMap(entities[0]->anticipateMove(deltaTime)))
        {
            entities[0]->move(-deltaTime);
            if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) camera->resetYaw();
        }

    }

    if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (keyPressed[GLFW_KEY_D]) entities[0]->turn(TURN_SPEED * deltaTime);
        if (keyPressed[GLFW_KEY_A]) entities[0]->turn(TURN_SPEED * -deltaTime);
    }
}

int main()
{
    window = new Window(800, 600);

    entities.push_back(new Entity(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/doublecube.fbx"));
    entities.push_back(new Entity(1, glm::vec3(5.0f, 0.0f, 5.0f), "models/fbx/doublecube.fbx"));

    //entities.push_back(new Entity(2, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/ground.fbx"));

    //glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(entities[0]->getPositionP(), entities[0]->getPYaw(), &keyPressed);

    glm::mat4* view = camera->getViewMatrixP();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 300.0f);

    //--- Création des shaders ---//    
    Shader shaders        = Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", view, &projection);
    Shader simple_shaders = Shader("shaders/simple_vertex_shader.glsl", "shaders/simple_fragment_shader.glsl", view, &projection);

    GLFWwindow* glfwWindow = window->getGLFWWindow();
    glfwSetKeyCallback(glfwWindow, keyCallback);

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glIsEnabled(GL_DEPTH_TEST)) std::cout << "Depth test is enabled."     << std::endl;
    else                            std::cout << "Depth test is not enabled." << std::endl;

    largeTile = new LargeTile(0, 0, "heightmaps/h1.exr");

    //readEXR("heightmaps/h1.exr");

    auto  startTime    = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame    = glfwGetTime(), deltaTime = 0, now = 0;
    
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
        if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT))
        {
            if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT))
            {
                GLfloat offsetYaw = camera->getYaw() - entities[0]->getYaw();
                //std::cout << offsetYaw << std::endl;
                entities[0]->turn(offsetYaw);

                if (checkHeightMap(entities[0]->anticipateMove(deltaTime)))
                {
                    entities[0]->move(deltaTime);
                }
            }

            if      (window->getXChange() > 0) entities[0]->turn(camera->getSensitivity() * deltaTime);
            else if (window->getXChange() < 0) entities[0]->turn(camera->getSensitivity() * -deltaTime);
        }

        processKeyPressed(glfwWindow, deltaTime);

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue(), deltaTime);

        // Effacer le buffer de couleur et de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Utiliser le programme de shaders
        shaders.use();
        
        //glUniform3fv(ColorLoc, 1, glm::value_ptr(color));

        //Passer les matrices de vue et de projection aux shaders
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //renderTerrain();
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelmtx));

        //chunk.RenderMesh();
        //printMatrix(*view);

        for (Entity* e : entities)
            if(e) e->render(shaders.modelLoc, shaders.bonesTransformsLoc, timeSinceStart);

        //--- Render terrain ---//
        glm::mat4 modelmtx = glm::mat4(1.0f);
        simple_shaders.use();
        glUniformMatrix4fv(simple_shaders.modelLoc, 1, GL_FALSE, glm::value_ptr(modelmtx));
        
        largeTile->render();
        


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

    return 0;
}
