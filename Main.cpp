#pragma once
#include <vector>
#include <chrono>

#include "Window.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"

#include "PhysicsEngine.h"
#include "Map.h"

#include "stb_image.h"

#include <tinyexr.h>

Window*         window  = nullptr;
Camera*         camera  = nullptr;
Game::Map*      world = nullptr;
PhysicsEngine   physics;
//float r = 0.5f, g = 0.5f, b = 0.5f;
std::map<char, bool> keyPressed;
std::map<int , bool> mousePressed;

std::vector<Entity*> entities;

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

    int chunkLength = (LARGETILE_SIZE * CHUNK_ARR_SIZE);

    Chunk*     chunk = world->getChunk(    (int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);
    LargeTile* lt    = chunk->getLargeTile((int)element->getPosition().z / chunkLength, (int)element->getPosition().x / chunkLength);

    int z = (int)elementPos->z % LARGETILE_SIZE,
        x = (int)elementPos->x % LARGETILE_SIZE;

    int nextZ = (z < LARGETILE_SIZE - 1) ? z + 1 : z - 1,
        nextX = (x < LARGETILE_SIZE - 1) ? x + 1 : x - 1;

    //std::cout << "NextZ: " << nextZ << " ... " << "NextX: " << nextX << std::endl;

    GLfloat interpolatedGroundY = 0.0f, y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;

    y1 = lt->getVertex(nextZ, x).y;      y2 = lt->getVertex(nextZ, nextX).y;
    y3 = lt->getVertex(z    , x).y;      y4 = lt->getVertex(z    , nextX).y;

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if      (action == GLFW_PRESS)      mousePressed[button] = true;
    else if (action == GLFW_RELEASE) 
    {
        mousePressed[button] = false;

        if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT && !keyPressed['W'] && !keyPressed['S']) entities[0]->setMove(false);
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

    entities.push_back(new Entity(0, glm::vec3(1536, 5.0f, 1024), "models/fbx/doublecube.fbx"));
    entities.push_back(new Entity(1, glm::vec3(300.0f, 6.5f, 300.0f), "models/fbx/doublecube.fbx"));

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

    //Loading chunks
    LargeTile*** largeTiles = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles[y] = new LargeTile * [CHUNK_ARR_SIZE];

    //Chunk 0 0                                                                     
    largeTiles[0][0] = new LargeTile(0, 0, 0, 0, "h1.exr", "h1.png", shaders);
    largeTiles[1][0] = new LargeTile(1, 0, 0, 0, "h1.exr", "h2.png", shaders);
    largeTiles[0][1] = new LargeTile(0, 1, 0, 0, "h1.exr", "h3.png", shaders);
    largeTiles[1][1] = new LargeTile(1, 1, 0, 0, "h1.exr", "h4.png", shaders);

    world->setChunk(0, 0, new Chunk(0, 0, shaders, largeTiles));

    //Chunk 0 1
    LargeTile*** largeTiles2 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles2[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles2[0][0] = new LargeTile(0, 0, 0, 1, "h1.exr", "h1.png", shaders);
    largeTiles2[1][0] = new LargeTile(1, 0, 0, 1, "h1.exr", "h2.png", shaders);
    largeTiles2[0][1] = new LargeTile(0, 1, 0, 1, "h1.exr", "h3.png", shaders);
    largeTiles2[1][1] = new LargeTile(1, 1, 0, 1, "h1.exr", "h4.png", shaders);

    world->setChunk(0, 1, new Chunk(0, 1, shaders, largeTiles2));

    //Chunk 1 0
    LargeTile*** largeTiles3 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles3[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles3[0][0] = new LargeTile(0, 0, 1, 0, "h1.exr", "h1.png", shaders);
    largeTiles3[1][0] = new LargeTile(1, 0, 1, 0, "h1.exr", "h2.png", shaders);
    largeTiles3[0][1] = new LargeTile(0, 1, 1, 0, "h1.exr", "h3.png", shaders);
    largeTiles3[1][1] = new LargeTile(1, 1, 1, 0, "h1.exr", "h4.png", shaders);

    world->setChunk(1, 0, new Chunk(1, 0, shaders, largeTiles3));

    LargeTile*** largeTiles4 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles4[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles4[0][0] = new LargeTile(0, 0, 1, 1, "h1.exr", "h1.png", shaders);
    largeTiles4[1][0] = new LargeTile(1, 0, 1, 1, "h1.exr", "h2.png", shaders);
    largeTiles4[0][1] = new LargeTile(0, 1, 1, 1, "h1.exr", "h3.png", shaders);
    largeTiles4[1][1] = new LargeTile(1, 1, 1, 1, "h1.exr", "h4.png", shaders);

    world->setChunk(1, 1, new Chunk(1, 1, shaders, largeTiles4));




    auto  startTime    = std::chrono::high_resolution_clock::now();
    float currentFrame = 0, animationTime = 0, timeSinceStart = 0,
          lastFrame    = glfwGetTime(), deltaTime = 0, now = 0;

    Texture textureTemp("textures/basic_texture_1.png");

    GLint maxTextures;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
    std::cout << "Max textures supported: " << maxTextures << std::endl;
    
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
            
            if(checkHeightMap(entities[0], entities[0]->anticipateMove(deltaTime * factor)))
            {
                entities[0]->move(deltaTime * factor);
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

        //--- Render terrain ---//
        //glm::mat4 modelmtx = glm::mat4(1.0f);
        //shaders[LARGETILES_SHADERS].use();
        //glUniformMatrix4fv(shaders[LARGETILES_SHADERS].modelLoc, 1, GL_FALSE, glm::value_ptr(modelmtx));
        
        //largeTile->render();
        //chunk->render();
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

    if (world)
    {
        delete world;
        world = nullptr;
    }

    return 0;
}
