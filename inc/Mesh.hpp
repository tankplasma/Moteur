#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture;
};

class Mesh
{
    public:

        Mesh();

        void draw(glm::mat4& mvpContext);
        void setup(vector<Vertex> vertices, vector<GLuint> indices);
        void setTextureID(GLuint textureID);
        void setShaders(GLuint shaders);
        void setSkyBox();
        void loadTexture(const string fileName);

    private:
    
        vector<Vertex> mVertices;
        vector<GLuint> mIndices;
        GLuint mTextureID;

        // Identifiant "Vertex Array Object" (enregistré dans OpenGL)
        GLuint mVAO;
        // Identifiant "Vertex Buffer Object" (enregistré dans OpenGL)
        GLuint mVBO;
        // Identifiant "Element Buffer Object" (enregistré dans OpenGL)
        GLuint mEBO;

        GLuint mShaders;

        bool mIsSkyBox;

};

class Pyramide : public Mesh { public: Pyramide(); };
class PyramideCouleur : public Mesh { public: PyramideCouleur(); };

class CubeDerriere : public Mesh { public: CubeDerriere(); };
class CubeGauche : public Mesh { public: CubeGauche(); };
class CubeDevant : public Mesh { public: CubeDevant(); };
class CubeDroite : public Mesh { public: CubeDroite(); };
class CubeHaut : public Mesh { public: CubeHaut(); };
class CubeBas : public Mesh { public: CubeBas(); };

class CubeCouleur : public Mesh { public: CubeCouleur(); };

#endif // MESH_HPP
