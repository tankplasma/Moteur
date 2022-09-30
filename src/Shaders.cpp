#include "../inc/Shaders.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

GLuint Shaders::generateProgram(string vertexGlsl, string fragmentGlsl)
{
    GLuint vertexBinary = compileGLSL(vertexGlsl, GL_VERTEX_SHADER);
//        = compileGLSL("../rsc/shaders/vertex_tex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentBinary = compileGLSL(fragmentGlsl, GL_FRAGMENT_SHADER);
//        = compileGLSL("../rsc/shaders/fragment_tex.glsl", GL_FRAGMENT_SHADER);
  
    // Demande à OpenGL d'enregistrer et fournir un identifiant de programme GPU
    GLuint shadersProgram = glCreateProgram();
    // Génération du programme pour le processeur graphique
    glAttachShader(shadersProgram, vertexBinary);
    glAttachShader(shadersProgram, fragmentBinary);
    glLinkProgram(shadersProgram);
    // Validation de l'édition de lien
    checkOpenGLError();
    GLint status;
    glGetProgramiv(shadersProgram, GL_LINK_STATUS, &status);
    if (status != 1) {
        cout << "Erreur de link ..." << endl;
        // En cas d'erreur, récupération des logs de l'erreur
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetProgramiv(shadersProgram, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = new char[len];
            glGetProgramInfoLog(shadersProgram, len, &chWrittn, log);
            cout << log << endl;
            delete log;
        } else {
            cout << "... (no log found)" << endl;
        }
        exit(EXIT_FAILURE);
    } else {
        cout << "Programme OK" << endl;
    }

    // Nettoyage...
    glDetachShader(shadersProgram, vertexBinary);
    glDetachShader(shadersProgram, fragmentBinary);
    glDeleteShader(vertexBinary);
    glDeleteShader(fragmentBinary);

    // Retourne l'identifiant du programme GPU généré
    return shadersProgram;
}

GLuint Shaders::compileGLSL(string filePath, GLenum shaderType)
{
    // Chargement du fichier GLSL
    ifstream fileStream(filePath, ios::in);
    if ( ! fileStream.is_open()) {
        cout << "Impossible d'acceder a " << filePath << " !" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Lecture de " << filePath << "..." << endl;
    stringstream contentStream;
    contentStream << fileStream.rdbuf();
    string contentString = contentStream.str();
    const char* glslSource = contentString.c_str();

    // Demande à OpenGL d'enregistrer et fournir un identifiant de "shader"
    GLuint shader = glCreateShader(shaderType);
    // Enregistre le code source GLSL dans le "shader"
    glShaderSource(
        shader, 
        // Un seul code source (possibilité de fournir plusieurs codes sources)
        1, 
        // Tableau des codes sources (remplacé ici par l'adresse de la variable)
        &glslSource, 
        // "NULL" indique que tous les sources sonts des "C-strings"
        NULL
        );

    // Lancement de la compilation du code source GLSL
    glCompileShader(shader);
    // Validation de la compilation
    checkOpenGLError();
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        cout << "Erreur de compilation de " << filePath << "..." << endl;
        // En cas d'erreur, récupération des logs de l'erreur
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char *)malloc(len);
            log = new char[len];
            glGetShaderInfoLog(shader, len, &chWrittn, log);
            cout << log << endl;
            delete log;
        } else {
            cout << "... (no log found)" << endl;
        }
        exit(EXIT_FAILURE);
    } else {
        cout << "Compilation de " << filePath << " OK" << endl;
    }

    return shader;
}

void Shaders::checkOpenGLError() {
    while (true)
    {
        int glErr = glGetError();
        if (glErr == GL_NO_ERROR) 
            break;
        cout << "glError: " << glErr <<endl;
    }
}
