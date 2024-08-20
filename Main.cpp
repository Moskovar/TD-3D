#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"

#include "PhysicsEngine.h"

#include "stb_image.h"

Window* window = nullptr;
Camera* camera = nullptr;
PhysicsEngine physics;
//float r = 0.5f, g = 0.5f, b = 0.5f;
std::map<char, bool> keyPressed;

std::vector<Entity*> entities;

GLuint vao, vbo, ibo;

struct DrawingVertex
{
    GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
    unsigned int indice = 0;
};

// Allocation dynamique pour un tableau 2D
DrawingVertex** vertices = nullptr;

// Affichage de la matrice
void printMatrix(const glm::mat4& mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void generateTerrainMesh(DrawingVertex** vertices, int width, int height, const char* heightmapPath) 
{
    // Charger l'image PNG
    int imgWidth, imgHeight, channels; 
    unsigned char* heightmapData = stbi_load(heightmapPath, &imgWidth, &imgHeight, &channels, 1); // 1 pour charger en niveau de gris

    if (!heightmapData) {
        std::cerr << "Erreur de chargement de l'image heightmap." << std::endl;
        return;
    }

    // Vérifiez que la largeur et la hauteur de l'image correspondent à la taille attendue
    if (imgWidth != width || imgHeight != height) {
        std::cerr << "Dimensions de l'image heightmap ne correspondent pas." << std::endl;
        stbi_image_free(heightmapData);
        return;
    }

    unsigned int indice = 0;
    // Générer le maillage de terrain
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Lire la valeur de hauteur du tableau heightmapData
            GLfloat heightValue = heightmapData[y * width + x];
            //std::cout << heightValue << std::endl;
            GLfloat inGameMaxHeight = 100.0f;
            // Définir la position du vertex en fonction de la hauteur
            vertices[y][x] = { (GLfloat)x, heightValue / 255.0f * inGameMaxHeight, (GLfloat)y, indice };
            indice++;
        }
    }

    // Libérer la mémoire de l'image
    if (heightmapData)
        stbi_image_free(heightmapData);    
}

void checkHeightMap()
{
    glm::vec3 playerPos = entities[0]->getPosition();

    if (playerPos.x >= 0 && playerPos.x < 32 && playerPos.z >= 0 && playerPos.z < 32)//Si le joueur est dans la heightmap
    {
        std::cout << vertices[(int)playerPos.y][(int)playerPos.x].y - entities[0]->getPosition().y << std::endl;

        GLfloat interpolatedY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

        y1 = vertices[(int)playerPos.y + 1][(int)playerPos.x].y;    y2 = vertices[(int)playerPos.y + 1][(int)playerPos.x + 1].y;
        y3 = vertices[(int)playerPos.y][(int)playerPos.x].y;        y4 = vertices[(int)playerPos.y][(int)playerPos.x + 1].y;

        entities[0]->moveUp((y1 + y2 + y3 + y4) / 4.0f);
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

void applyGravity(GLfloat deltaTime)
{
    //std::cout << entities[1]->getRHitbox().max_point.y << std::endl;
    //std::cout << entities[1]->getRHitbox().max_point.x << std::endl;
    for (int i = 1; i < entities.size(); i++)
        if (checkCollision(entities[0]->getRHitbox(), entities[i]->getRHitbox())) return;
        
    entities[0]->fall(deltaTime);
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
        entities[0]->move(deltaTime);
        if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) camera->resetYaw();

        checkHeightMap();
    }

    if (keyPressed[GLFW_KEY_S])
    {
        entities[0]->move(-deltaTime);
        if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) camera->resetYaw();

        checkHeightMap();
    }

    if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (keyPressed[GLFW_KEY_D]) entities[0]->turn(TURN_SPEED * deltaTime);
        if (keyPressed[GLFW_KEY_A]) entities[0]->turn(TURN_SPEED * -deltaTime);
    }
}

int main()
{
    vertices = new DrawingVertex * [32];
    for (int i = 0; i < 32; ++i) 
    {
        vertices[i] = new DrawingVertex[32] {0.0f};
    }

    //// Initialisation avec des zéros
    //for (int y = 0; y < HEIGHT; ++y) {
    //    for (int x = 0; x < WIDTH; ++x) {
    //        vertices2[x][y] = 0.0f;
    //    }
    //}

    generateTerrainMesh(vertices, 32, 32, "heightmaps/h1.png");

    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 32; ++x)
            std::cout << vertices[y][x].x << " ... " << vertices[y][x].y << " ... " << vertices[y][x].z << std::endl;

    window = new Window(800, 600);

    entities.push_back(new Entity(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/doublecube.fbx"));
    entities.push_back(new Entity(1, glm::vec3(5.0f, 0.0f, 5.0f), "models/fbx/doublecube.fbx"));

    //entities.push_back(new Entity(2, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/ground.fbx"));

    //glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(entities[0]->getPositionP(), entities[0]->getPYaw(), &keyPressed);

    glm::mat4* view = camera->getViewMatrixP();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 100.0f);

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

    std::vector<DrawingVertex> v_drawingVertices = {};
    std::vector<unsigned int>  v_indices = {};

    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 32; ++x)
        {
            v_drawingVertices.push_back(vertices[y][x]);
            if (x < 31 && y < 31)
            {
                //triangle de gauche
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y][x + 1].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
            }

            if (x > 0 && y < 31)
            {
                ////triangle de droite
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
                v_indices.push_back(vertices[y + 1][x - 1].indice);
            }
        }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v_drawingVertices.size() * sizeof(DrawingVertex), v_drawingVertices.data(), GL_STATIC_DRAW);

    // Lier et remplir l'EBO (Element Buffer Object)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_indices.size() * sizeof(unsigned int), v_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawingVertex), (void*)offsetof(DrawingVertex, x));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



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

        //applyGravity(deltaTime);


        //--- Personnage ---//
        if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT))
        {
            if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT))
            {
                GLfloat offsetYaw = camera->getYaw() - entities[0]->getYaw();
                //std::cout << offsetYaw << std::endl;
                entities[0]->turn(offsetYaw);
                entities[0]->move(deltaTime);

                checkHeightMap();
            }

            if      (window->getXChange() > 0) entities[0]->turn(camera->getSensitivity() * deltaTime);
            else if (window->getXChange() < 0) entities[0]->turn(camera->getSensitivity() * -deltaTime);
        }

        processKeyPressed(glfwWindow, deltaTime);

        //--- Caméra ---//
        camera->mouseControl(window->getGLFWWindow(), window->getXChange(), window->getYChange(), window->getScrollValue(), deltaTime);

        camera->update();

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


        glm::mat4 modelmtx = glm::mat4(1.0f);
        simple_shaders.use();
        glUniformMatrix4fv(simple_shaders.modelLoc, 1, GL_FALSE, glm::value_ptr(modelmtx));
        //printMatrix(*view);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


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

    delete vertices;
    vertices = nullptr;

    return 0;
}
