#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Model.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

    public:

        Camera();

        void setProjection(glm::mat4& projection);
        void setView(glm::mat4& view);
        glm::mat4& getView();
        glm::mat4 getVPmatrix();

    private:

        glm::mat4 mProjection;
        glm::mat4 mView;

};

#endif // CAMERA_HPP
