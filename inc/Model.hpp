#ifndef MODEL_HPP
#define MODEL_HPP

#include <Mesh.hpp>

#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
using namespace std;

class ChildModel;

class Model
{

    public:
        
        Model();

        void addMesh(Mesh& mesh);
        void loadSurface(const string& path, GLuint shaders);
        void addChild(Model& model, glm::mat4& modelMatrix);
        vector<Model*>& getChilden();
        void setModelMatrix(glm::mat4& modelMatrix);
        glm::mat4& getModelMatrix();
        virtual void processEvents();
        void draw(glm::mat4& mvpContext);

    private:

        vector<Mesh> mSurface;
        vector<Model*> mChildren;
        glm::mat4 mModelMatrix;

        void processNode(aiNode *node, const aiScene *scene, GLuint shaders);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene, GLuint shaders);

};

class PyraModel : public Model { public: virtual void processEvents(); };

class SkyBox : public Model { public: virtual void processEvents(); };

class RigidModel : public Model {

     public: 
     
        void setBoxBody(
            float halfWidth, float halfHeight, float halfDepth,
            float x, float y, float z,
            float mass
            ) ;
        btRigidBody* getRigidBody_ptr();

        virtual void processEvents(); 

    private:

        btRigidBody* mRigidBody_ptr;

};

#endif // MODEL_HPP
