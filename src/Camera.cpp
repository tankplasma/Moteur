#include "../inc/Camera.hpp"

#include <GL/glew.h>

// Constructeur par défaut avec des matrices identités
Camera::Camera()
: mProjection( glm::mat4(1) )
, mView( glm::mat4(1) )
{}

void Camera::setProjection(glm::mat4& projection) { mProjection = projection; }

void Camera::setView(glm::mat4& view) { mView = view; }

glm::mat4& Camera::getView() { return mView; }

glm::mat4 Camera::getVPmatrix() { return mProjection * mView; }
