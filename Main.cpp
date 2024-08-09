#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"


#define TURN_SPEED          100.0//character turn speed

Window* window = nullptr;
Camera* camera = nullptr;
float r = 0.5f, g = 0.5f, b = 0.5f;

std::vector<Entity*> entities;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        switch (key) 
        {
            case GLFW_KEY_W: entities[0]->directionPressed(GLFW_KEY_W);     break;
            case GLFW_KEY_S: entities[0]->directionPressed(GLFW_KEY_S);     break;
            case GLFW_KEY_D: entities[0]->directionPressed(GLFW_KEY_D);     break;
            case GLFW_KEY_A: entities[0]->directionPressed(GLFW_KEY_A);     break;
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);   break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_W: entities[0]->directionReleased(GLFW_KEY_W);     break;
            case GLFW_KEY_S: entities[0]->directionReleased(GLFW_KEY_S);     break;
            case GLFW_KEY_D: entities[0]->directionReleased(GLFW_KEY_D);     break;
            case GLFW_KEY_A: entities[0]->directionReleased(GLFW_KEY_A);     break;
        }
    }
}

void processKeyPressed(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        entities[0]->move(deltaTime);
        camera->resetYaw();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        entities[0]->move(-deltaTime);
        camera->resetYaw();
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        entities[0]->turn(TURN_SPEED * deltaTime);
        camera->addYaw(TURN_SPEED * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        entities[0]->turn(TURN_SPEED * -deltaTime);
        camera->addYaw(TURN_SPEED * -deltaTime);
    }
}

int main()
{
    window = new Window(800, 600);

    entities.push_back(new Entity(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/doublecube.fbx"));
    //entities.push_back(new Entity(1, glm::vec3(0.0f,  0.0f, 0.0f), "models/obj/r1.obj"));

    entities.push_back(new Entity(2, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/ground.fbx"));
    //glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(entities[0]->getPositionP());
    //--- Création des shaders ---//
    GLuint shaderProgram = 0;
    // Obtenir l'emplacement des uniforms
    GLuint ColorLoc = 0, viewLoc = 0, projLoc = 0, modelLoc = 0, bonesTransformsLoc = 0;
    {
        string vertexCode = "shaders/vertex_shader.glsl";// Lire le code du vertex shader ├á partir du fichier vertex_shader.glsl
        string fragmentCode = "shaders/fragment_shader.glsl";// Lire le code du fragment shader ├á partir du fichier fragment_shader.glsl
        Shader shaders = Shader(vertexCode, fragmentCode);
        shaderProgram = shaders.getShaderProgram();
        ColorLoc = glGetUniformLocation(shaderProgram, "color");
        modelLoc = glGetUniformLocation(shaderProgram, "model");
        viewLoc  = glGetUniformLocation(shaderProgram, "view");
        projLoc  = glGetUniformLocation(shaderProgram, "projection");
        bonesTransformsLoc = glGetUniformLocation(shaderProgram, "bonesTransform");
    }

    GLint idLoc = glGetUniformLocation(shaderProgram, "id");//debug

    //--- Couleurs du modèle ---//
    glm::vec3 color(r, g, b);

    //Matrice de la caméra
    glm::mat4* view = camera->getViewMatrixP();

    //Matrice de projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 100.0f);

    //Exemples transformations
    {
        //glm::vec3 scale(0.1f, 0.1f, 0.1f); // Echelle de 1x en x, 2x en y, et 1x en z
        //model = glm::scale(model, scale);

        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    GLFWwindow* glfwWindow = window->getGLFWWindow();
    glfwSetKeyCallback(glfwWindow, keyCallback);

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glIsEnabled(GL_DEPTH_TEST)) std::cout << "Depth test is enabled."     << std::endl;
    else                            std::cout << "Depth test is not enabled." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame = glfwGetTime(), deltaTime = 0, now = 0;
    
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow))
    {   //AnimationTime
        auto currentTime = std::chrono::high_resolution_clock::now();
        timeSinceStart = std::chrono::duration<float>(currentTime - startTime).count();

        //DeltaTime
        now       = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        processKeyPressed(glfwWindow, deltaTime);

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue(), deltaTime);

        if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT))
        {
            if      (window->getXChange() > 0)
            {
                entities[0]->turn(camera->getSensitivity() * deltaTime);
                camera->addYaw(camera->getSensitivity() * deltaTime);
            }
            else if (window->getXChange() < 0)
            {
                entities[0]->turn(camera->getSensitivity() * -deltaTime);
                camera->addYaw(camera->getSensitivity() * -deltaTime);
            }
        }

        camera->update();

        // Effacer le buffer de couleur et de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Utiliser le programme de shaders
        glUseProgram(shaderProgram);

        color.x = r;
        color.y = g;
        color.z = b;
        
        glUniform3fv(ColorLoc, 1, glm::value_ptr(color));

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (Entity* e : entities)
            e->render(modelLoc, bonesTransformsLoc, timeSinceStart, idLoc);

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
