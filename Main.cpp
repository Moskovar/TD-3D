#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"


Window* window = nullptr;
float r = 0.5f, g = 0.5f, b = 0.5f;

std::vector<Entity*> entities;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) 
    {
        switch (key) {
        case GLFW_KEY_W:           entities[1]->addZ(-5);                         break;
        case GLFW_KEY_S:           entities[1]->addZ(5);                         break;
        case GLFW_KEY_A:           entities[1]->addX(-5);                         break;
        case GLFW_KEY_D:           entities[1]->addX(5);                         break;
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);    break;
        case GLFW_KEY_F1:     r = (r < 1.0f) ? r + 0.1 : 1.0;            break;
        case GLFW_KEY_F2:     g = (g < 1.0f) ? g + 0.1 : 1.0;            break;
        case GLFW_KEY_F3:     b = (b < 1.0f) ? b + 0.1 : 1.0;            break;
        case GLFW_KEY_F4:     r = (r > 1.0f) ? r - 0.1 : 0.0;            break;
        case GLFW_KEY_F5:     g = (g > 1.0f) ? g - 0.1 : 0.0;            break;
        case GLFW_KEY_F6:     b = (b > 1.0f) ? b - 0.1 : 0.0;            break;
        }
    }
}

int main()
{
    window = new Window(800, 600);


    entities.push_back(new Entity(1, glm::vec3(0.0f,  0.0f, 0.0f), "models/fbx/doublecube.fbx"));
    entities.push_back(new Entity(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/cubeman.fbx"));

    entities.push_back(new Entity(2, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/ground.fbx"));
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera camera(entities[1]->getPositionP());
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

    GLint idLoc = glGetUniformLocation(shaderProgram, "id");


    //--- Couleurs du modèle ---//
    glm::vec3 color(r, g, b);

    //Matrice de la caméra
    glm::mat4* view = camera.getViewMatrixP();

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

    if (glIsEnabled(GL_DEPTH_TEST)) {
        std::cout << "Depth test is enabled." << std::endl;
    }
    else {
        std::cout << "Depth test is not enabled." << std::endl;
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    float lastFrame = glfwGetTime(), currentFrame = 0, animationTime = 0, deltaTime = 0;
    
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - startTime).count();

        camera.mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue());

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

        for(Entity* e : entities)
            e->render(modelLoc, bonesTransformsLoc, deltaTime, idLoc);

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
    
    for(Entity* e : entities)
        if (e) 
        {
            delete e;
            e = nullptr;
        }

    return 0;
}
