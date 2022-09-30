#ifndef OPGLTERM_HPP
#define OPGLTERM_HPP

#include <Scene.hpp>
#include <Camera.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
using namespace std;

class OpglTerm
{
    public:

        static OpglTerm context;

        OpglTerm();
        ~OpglTerm();

        void initWindow(int width, int height, const string& title);
        float getFPS();
        GLFWwindow* getWindow_ptr();
        Scene& getScene();
        Camera& getCamera();
        void runInteraction();

    private:

        bool mIsRunning;
        float mFPS;
        GLFWwindow* mWindow_ptr;
        Scene mScene;
        Camera mCamera;

        void processEvents();
        void renderWindow();
};

#endif // OPGLTERM_HPP
