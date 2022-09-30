#ifndef SCENE_HPP
#define SCENE_HPP

#include <Model.hpp>

#include <bullet/btBulletDynamicsCommon.h>

class Scene {

    public:

        Scene();
        
        void addObject(Model& model, glm::mat4& modelMatrix);
        vector<Model*>& getObjects();
        void processEvents();
        void draw(glm::mat4& vpMatrix);
        btDiscreteDynamicsWorld* getDynamicsWolrd_ptr();

    private:

        Model mRoot;
        btDiscreteDynamicsWorld* mDynamicsWolrd_ptr;
        
};

#endif // SCENE_HPP