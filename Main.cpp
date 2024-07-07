#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include "stb_image.h"

#include <iostream>
#include <vector>
#include <chrono>

#include "Model.h"

#include "Shader.h"

using namespace std;

GLFWwindow* window = nullptr;
float r = 0.5f, g = 0.5f, b = 0.5f,
x = 0.0f, y = 0.0f, z = 0.0f;

// Fonction de rappel pour les erreurs GLFW
void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W:      z += 0.1f;                                 break;
        case GLFW_KEY_S:      z -= 0.1f;                                 break;
        case GLFW_KEY_A:      x += 0.1f;                                 break;
        case GLFW_KEY_D:      x -= 0.1f;                                 break;
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

GLFWwindow* initialize()
{
    // Initialiser GLFW
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // Créer une fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Setup Test", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    //--- Configuration de GFLW ---//
    {
        // D├®finir la version majeure du contexte OpenGL que nous souhaitons utiliser (ici, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

        // D├®finir la version mineure du contexte OpenGL que nous souhaitons utiliser (ici, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Sp├®cifier le profil OpenGL ├á utiliser. Le profil "Core" ne comporte que les fonctionnalit├®s de base d'OpenGL, 
        // sans les anciennes fonctionnalit├®s obsol├¿tes. Cela est recommand├® pour les nouvelles applications.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Activer la compatibilit├® avec les versions ant├®rieures d'OpenGL sur macOS, 
        // car macOS requiert ce param├¿tre pour les versions r├®centes d'OpenGL. 
        // Cela permet aux programmes utilisant des fonctionnalit├®s modernes d'OpenGL de fonctionner correctement sur macOS.
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }
    //--- Fin configuration GLFW  --- //

    //On choisit sur quelle feuille on dessine
    glfwMakeContextCurrent(window);

    // Initialiser GLEW
    glewExperimental = GL_TRUE; // N├®cessaire pour utiliser les fonctionnalit├®s modernes d'OpenGL
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void clear()
{
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

int main()
{
    window = initialize();
    if (!window)
    {
        cout << "    ---------------------" <<
            endl << "/!\\ Initialization failed /!\\" <<
            endl << "    ---------------------" << endl;

        return -1;
    }

    Model character("models/fbx/triangles.fbx");

    //--- Création des shaders ---//
    GLuint shaderProgram = 0;
    // Obtenir l'emplacement des uniforms
    GLint ColorLoc = 0, viewLoc = 0, projLoc = 0, modelLoc = 0;
    {
        string vertexCode = "shaders/vertex_shader.glsl";// Lire le code du vertex shader ├á partir du fichier vertex_shader.glsl
        string fragmentCode = "shaders/fragment_shader.glsl";// Lire le code du fragment shader ├á partir du fichier fragment_shader.glsl
        Shader shaders = Shader(vertexCode, fragmentCode);
        shaderProgram = shaders.getShaderProgram();
        ColorLoc = glGetUniformLocation(shaderProgram, "color");
        modelLoc = glGetUniformLocation(shaderProgram, "model");
        viewLoc  = glGetUniformLocation(shaderProgram, "view");
        projLoc  = glGetUniformLocation(shaderProgram, "projection");
    }

    glm::vec3 pos(x, y, z);
    glm::vec3 color(r, g, b);

    // Définir la fonction de rappel pour les entrées clavier
    glfwSetKeyCallback(window, keyCallback);

    //Position de la caméra
    glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, -20.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    //Matrice du model
    glm::mat4 model = glm::mat4(1.0f); // Matrice de modèle d'identité

    //Matrice de vue
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    //Matrice de projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
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

    int c = 0;//debug
    //Boucle de rendu
    while (!glfwWindowShouldClose(window))
    {
        c++;//debug
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
        if (animation) animation->animate(animationTime);

        //--- Caméra ---//

        //Passer les matrices de vue et de projection aux shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        character.renderModel();

        //Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Nettoyer et quitter
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
