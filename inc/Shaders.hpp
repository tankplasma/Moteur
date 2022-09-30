#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <GL/glew.h>

#include <string>
using namespace std;

class Shaders
{

    public:

        static GLuint generateProgram(string vertexGlsl, string fragmentGlsl);

    private:

        static GLuint compileGLSL(string filePath, GLenum shaderType);
        static void checkOpenGLError();

};

#endif // SHADERS_HPP
