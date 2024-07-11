#include "Window.h"

Window::Window(short width, short height)
{
	this->width = width;
	this->height = height;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(0);
    }

    // Créer une fenêtre GLFW
    window = glfwCreateWindow(width, height, "OpenGL Development", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }

    //--- Configuration de GFLW ---//

    // Définir la version majeure du contexte OpenGL que nous souhaitons utiliser (ici, 3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    // Définir la version mineure du contexte OpenGL que nous souhaitons utiliser (ici, 3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Spécifier le profil OpenGL á utiliser. Le profil "Core" ne comporte que les fonctionnalités de base d'OpenGL, 
    // sans les anciennes fonctionnalités obsolètes. Cela est recommandé pour les nouvelles applications.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Activer la compatibilité avec les versions antérieures d'OpenGL sur macOS, 
    // car macOS requiert ce paramètre pour les versions récentes d'OpenGL. 
    // Cela permet aux programmes utilisant des fonctionnalités modernes d'OpenGL de fonctionner correctement sur macOS.
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    //--- Fin configuration GLFW  --- //

    //On choisit sur quelle feuille on dessine
    glfwMakeContextCurrent(window);

    createCallbacks();

    // Initialiser GLEW
    glewExperimental = GL_TRUE; // Nécessaire pour utiliser les fonctionnalités modernes d'OpenGL
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }

    if (!window)
    {
            cout << "    ---------------------" <<
            endl << "/!\\ Initialization failed /!\\" <<
            endl << "    ---------------------" << endl;

        exit(0);
    }

    glfwSetWindowUserPointer(window, this);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();

    cout << "||--- Window cleared ---||" << endl;
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void Window::handleScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    theWindow->scrollValue = yOffset;
}

void Window::createCallbacks()
{
    glfwSetErrorCallback(glfwErrorCallback);
    //glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(window, handleMouse);
    glfwSetScrollCallback(window, handleScroll);
}
