#include "../inc/Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <OpglTerm.hpp>

#include <iostream>
using namespace std;

Model::Model()
: mModelMatrix( glm::mat4(1.0) )
{}

void Model::addMesh(Mesh& mesh)
{
    mSurface.push_back(mesh);
}

void Model::loadSurface(const string& path, GLuint shaders)
{
    Assimp::Importer importer;
    const aiScene * scene 
        = importer.ReadFile(path, aiProcess_Triangulate);
    if (   ! scene 
        || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
        || ! scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    processNode(scene->mRootNode, scene, shaders);
    cout << endl;
}

void Model::addChild(Model& model, glm::mat4& modelMatrix)
{
    model.setModelMatrix(modelMatrix);
    mChildren.push_back(&model);
}

vector<Model*>& Model::getChilden() { return mChildren; }

void Model::setModelMatrix(glm::mat4& modelMatrix) 
{ mModelMatrix = modelMatrix; }

glm::mat4& Model::getModelMatrix() { return mModelMatrix; }

void Model::processEvents()
{
    for (Model* child_ptr: mChildren)
    {
        child_ptr->processEvents();
    }
}

void Model::draw(glm::mat4& mvpContext)
{
    for (Mesh mesh: mSurface) mesh.draw(mvpContext);
    for (Model* child_ptr: mChildren)
    {
        child_ptr->draw( mvpContext * child_ptr->getModelMatrix() );
    }
}

void Model::processNode(aiNode *node, const aiScene *scene, GLuint shaders) {
    cout << "processNode...";
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[ node->mMeshes[i] ];
        mSurface.push_back( processMesh(mesh, scene, shaders) );
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, shaders);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, GLuint shaders) {
    cout << "processMesh...";
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        //cout << vector.x << "#";
        vertex.position = vector;
        vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back( face.mIndices[j] );
        }
    }

    Mesh result;
    result.setup(vertices, indices);
    result.setShaders(shaders);
    return result;
}

void PyraModel::processEvents()
{
    // On fait tourner la pyramide à un tour par seconde
    float rotation = 360.0f / OpglTerm::context.getFPS();
    glm::mat4 modelMatrix = this->getModelMatrix();
    this->setModelMatrix( glm::rotate(
        modelMatrix, 
        glm::radians(rotation), 
        glm::vec3(0.0f, 1.0f, 0.0f)
        ) );
    // Propagation aux sous-modèles
    Model::processEvents();
}

void SkyBox::processEvents()
{
/*/
    // La position de la caméra est à l'inverse de la transformation "Vue"
    glm::mat4 camMatrix = glm::inverse(OpglTerm::context.getCamera().getView());
    // On positionne la SkyBox là où se trouve la caméra
    //glm::mat4 modelMatrix = this->getModelMatrix();
    //modelMatrix[3] = camMatrix[3]; // => Juste la position sans les rotations
    this->setModelMatrix(camMatrix);
/*/
    // Propagation aux sous-modèles
    Model::processEvents();
}

void RigidModel::setBoxBody(
    float halfWidth, float halfHeight, float halfDepth,
    float x, float y, float z,
    float mass
    )
{
    btCollisionShape* shape_ptr
        = new btBoxShape( btVector3(halfWidth, halfHeight, halfDepth) );
    btDefaultMotionState* motionState_ptr = new btDefaultMotionState(
        btTransform( btQuaternion(0.0f,0.0f,0.0f,1.0f), btVector3(x,y,z) )
    );
    btVector3 inertia(0.0f,0.0f,0.0f);
    shape_ptr->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyConsInfo
        (mass, motionState_ptr, shape_ptr, inertia);

    mRigidBody_ptr = new btRigidBody(rigidBodyConsInfo);

    mRigidBody_ptr->setRestitution(1.0f);
    mRigidBody_ptr->setFriction(1.0f);
    mRigidBody_ptr->setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
}

btRigidBody* RigidModel::getRigidBody_ptr()
{
    return mRigidBody_ptr;
}


void RigidModel::processEvents()
{
    btTransform transform;
    this->getRigidBody_ptr()->getMotionState()->getWorldTransform(transform);
    btVector3 translate = transform.getOrigin();
    glm::mat4 physMatrix = glm::translate(
        glm::mat4(1),
        glm::vec3( translate.getX(), translate.getY(), translate.getZ() )
    );

    this->setModelMatrix(physMatrix);

    // Propagation aux sous-modèles
    Model::processEvents();
}

