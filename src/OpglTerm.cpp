#include "../inc/OpglTerm.hpp"

#include <iostream>
using namespace std;

// Contexte global d'exécution de l'application pour OpenGL
OpglTerm OpglTerm::context;

OpglTerm::OpglTerm()
: mIsRunning(false)
, mFPS(60)
, mWindow_ptr(nullptr)
{
    // Initialisation de GLFW réalisant l'interface entre OpenGL et le système
    int status = glfwInit();
    if ( status == GLFW_FALSE ) {
        cout << "Erreur d'initialisation GLFW !" << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "Initialisation GLFW OK..." << endl;
    }
    // Version d'OpenGL utilisée : 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

OpglTerm::~OpglTerm()
{
    if (mWindow_ptr) glfwDestroyWindow(mWindow_ptr);
    glfwTerminate();
}

void OpglTerm::initWindow(int width, int height, const string& title)
{
    // Création de la fenêtre
    mWindow_ptr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (mWindow_ptr == NULL) {
        cout << "Impossible de creer la fenetre !" << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "Initialisation de la fenetre GLFW OK..." << endl;
    }

    // Active OpenGL sur cette fenêtre
    glfwMakeContextCurrent(mWindow_ptr);
    // Initialisation de GLEW donnant accès au GPU par l'intermédiare d'OpenGL
    // /!\ doit être fait après l'activation d'OpenGL sur une fenêtre du sytème
    int status = glewInit();
    if (status != GLEW_OK) {
        cout << "Erreur d'initialisation GLEW : " 
            << glewGetErrorString(status) 
            << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "Initialisation GLEW OK..." << endl;
    }
}

float OpglTerm::getFPS() { return mFPS; }

GLFWwindow* OpglTerm::getWindow_ptr() { return mWindow_ptr; }

Scene& OpglTerm::getScene() { return mScene; }

Camera& OpglTerm::getCamera() { return mCamera; }

void processKeyboard();
//void processMouseWeel(GLFWwindow* window, double xoffset, double yoffset);

void OpglTerm::runInteraction()
{
    //glfwSetScrollCallback(mWindow_ptr, processMouseWeel);
    mIsRunning = true;
    // Boucle d'interaction avec utilisateur
    // ... synchronisée avec le temps
    mFPS = 25.0f;
    float timePerFrame = 1.0f / mFPS ;
    float currentFrame;
    float deltaTime = 0;
    float lastFrame = glfwGetTime(); 
    while (mIsRunning)
    {
        currentFrame = glfwGetTime();
        deltaTime += currentFrame - lastFrame;
        lastFrame = currentFrame;
        while (deltaTime > timePerFrame)
        {
            deltaTime -= timePerFrame;
            this->processEvents();
        }
        this->renderWindow();
    }
    /*/ ... non synchonisée
    while (mIsRunning)
    {
        this->processEvents();
        this->renderWindow(); 
    }
    /*/
}

void OpglTerm::processEvents()
{
    // Récupère les interactions encore en attente
    glfwPollEvents();
    // Fin d'exécution si la fenêtre est fermée
    mIsRunning = ! glfwWindowShouldClose(mWindow_ptr);

    processKeyboard();
    mScene.processEvents();
}

void processKeyboard()
{
    GLFWwindow* window_ptr = OpglTerm::context.getWindow_ptr();
    glm::mat4 view = OpglTerm::context.getCamera().getView();
    if ( glfwGetKey(window_ptr, GLFW_KEY_SPACE ) == GLFW_PRESS )
        view = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, 0.1f)
            ) * view;
    if ( glfwGetKey(window_ptr, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS )
        view = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, -0.1f)
            ) * view;
    if ( glfwGetKey(window_ptr, GLFW_KEY_RIGHT ) == GLFW_PRESS )
        view = glm::rotate(
            glm::mat4(1.0f), 
            glm::radians(1.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
            ) * view;
    if ( glfwGetKey(window_ptr, GLFW_KEY_LEFT ) == GLFW_PRESS )
        view = glm::rotate(
            glm::mat4(1.0f), 
            glm::radians(-1.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
            ) * view;
    if ( glfwGetKey(window_ptr, GLFW_KEY_UP ) == GLFW_PRESS )
        view = glm::rotate(
            glm::mat4(1.0f), 
            glm::radians(-1.0f), 
            glm::vec3(1.0f, 0.0f, 0.0f)
            ) * view;
    if ( glfwGetKey(window_ptr, GLFW_KEY_DOWN ) == GLFW_PRESS )
        view = glm::rotate(
            glm::mat4(1.0f), 
            glm::radians(1.0f), 
            glm::vec3(1.0f, 0.0f, 0.0f)
            ) * view;
    OpglTerm::context.getCamera().setView(view);
}

/*/
void processMouseWeel(GLFWwindow* window, double xoffset, double yoffset)
{
    GLFWwindow* window_ptr = OpglTerm::context.getWindow_ptr();
    glm::mat4 view = OpglTerm::context.getCamera().getView();
    view = glm::rotate(
        glm::mat4(1.0f), 
        glm::radians( (float)(yoffset * 5) ), 
        glm::vec3(1.0f, 0.0f, 0.0f)
        ) * view;
    OpglTerm::context.getCamera().setView(view);
}
/*/

void OpglTerm::renderWindow()
{
    // Réinitialise le visualisateur OpenGL
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mScene.draw( mCamera.getVPmatrix() );

    glfwSwapBuffers(mWindow_ptr);
}
