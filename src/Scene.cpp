#include "../inc/Scene.hpp"

#include <OpglTerm.hpp>

Scene::Scene()
{
    btBroadphaseInterface* broadPhase_ptr =  new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration_ptr
        = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher_ptr
        = new btCollisionDispatcher(collisionConfiguration_ptr);
    btSequentialImpulseConstraintSolver* solver_ptr
        = new btSequentialImpulseConstraintSolver();

    mDynamicsWolrd_ptr = new btDiscreteDynamicsWorld
        (dispatcher_ptr, broadPhase_ptr, solver_ptr,collisionConfiguration_ptr);
    mDynamicsWolrd_ptr->setGravity( btVector3(0,-9.8f,0) );
}

void Scene::addObject(Model& model, glm::mat4& modelMatrix)
{
    mRoot.addChild(model, modelMatrix);
}

vector<Model*>& Scene::getObjects() { return mRoot.getChilden(); }

void Scene::processEvents()
{
    mDynamicsWolrd_ptr->stepSimulation( 1.0f / OpglTerm::context.getFPS() );
    mRoot.processEvents();
}

void Scene::draw(glm::mat4& vpMatrix)
{
    mRoot.draw(vpMatrix);
}

btDiscreteDynamicsWorld* Scene::getDynamicsWolrd_ptr()
{
    return mDynamicsWolrd_ptr;
}
