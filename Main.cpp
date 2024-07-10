#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"

#define NUM_BONES 100

Window* window = nullptr;
float r = 0.5f, g = 0.5f, b = 0.5f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W:                                    break;
        case GLFW_KEY_S:                                    break;
        case GLFW_KEY_A:                                    break;
        case GLFW_KEY_D:                                    break;
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
    window = new Window(1920, 1080);

    Model character("models/fbx/doublecube.fbx");
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

    //--- Création des shaders ---//
    GLuint shaderProgram = 0;
    // Obtenir l'emplacement des uniforms
    GLint ColorLoc = 0, viewLoc = 0, projLoc = 0, modelLoc = 0, bonesTransformsLoc = 0;
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

    //--- Couleurs du modèle ---//
    glm::vec3 color(r, g, b);

    //Matrice du model
    glm::mat4 model = glm::mat4(1.0f); // Matrice de modèle d'identité

    //Matrice de la caméra
    glm::mat4* view = camera.getViewMatrixP();

    //Matrice de projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
    //Exemples transformations
    {
        //glm::vec3 scale(0.1f, 0.1f, 0.1f); // Echelle de 1x en x, 2x en y, et 1x en z
        //model = glm::scale(model, scale);

        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    auto bones = character.getBones();

    Animation* animation = character.getAnimation(0);
    auto startTime = std::chrono::high_resolution_clock::now();
    float ticksPerSecond = 24;
    if (animation) ticksPerSecond = animation->getTicksPerSecond();
    float duration = animation->getDuration() / ticksPerSecond;
    GLFWwindow* glfwWindow = window->getGLFWWindow();
    // Définir la fonction de rappel pour les entrées clavier
    glfwSetKeyCallback(glfwWindow, keyCallback);
    float lastFrame = glfwGetTime(), currentFrame = 0, deltaTime = 0;
    int c = 0;//debug
    //Boucle de rendu
    while (!glfwWindowShouldClose(glfwWindow))
    {
        c++;//debug

        currentFrame = glfwGetTime();
        deltaTime    = currentFrame - lastFrame;

        camera.mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue());

        // Effacer l'écran
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Utiliser le programme de shaders
        glUseProgram(shaderProgram);

        color.x = r;
        color.y = g;
        color.z = b;

        glUniform3fv(ColorLoc, 1, glm::value_ptr(color));

        auto currentTime = std::chrono::high_resolution_clock::now();
        float timeElapsed = std::chrono::duration<float>(currentTime - startTime).count();
        float animationTime = fmod(timeElapsed * ticksPerSecond, duration);

        //--- Caméra ---//
        //printf("Bones size... %d\n", character.getBones().size());
        //for (auto bone : character.getBones())
        //{
        //    if (bone.second) printf("BoneName: %s\n", bone.first.c_str());
        //}

        glm::mat4 bonesTransform[NUM_BONES] = {};
        glm::mat4 mtx = glm::mat4(1.0f);
        character.getBone("Bone")->interpolateTransform(animationTime, bonesTransform, mtx);

        /*view = camera.getViewMatrixP();*/

        //Passer les matrices de vue et de projection aux shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(bonesTransformsLoc, NUM_BONES, GL_FALSE, &bonesTransform[0][0][0]);

        character.renderModel();

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
    
    return 0;
}
