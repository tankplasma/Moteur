#include <OpglTerm.hpp>
#include <Shaders.hpp>
#include <Camera.hpp>

#include <cstdlib>

int main(int argc, char *argv[])
{
    OpglTerm::context.initWindow(800, 600, "OpenGL");

    Scene& scene = OpglTerm::context.getScene();
    glm::mat4 modelMatrix;

    // Génération des programmes GLSL pour le GPU
    GLuint shaders = Shaders::generateProgram
        ("../rsc/shaders/vertex.glsl", "../rsc/shaders/fragment.glsl");
    GLuint shadersTex = Shaders::generateProgram
        ("../rsc/shaders/vertex_tex.glsl", "../rsc/shaders/fragment_tex.glsl");
    GLuint shadersSuz = Shaders::generateProgram
        ("../rsc/shaders/vertex_fch.glsl", "../rsc/shaders/fragment_fch.glsl");
/*/
    // Surface et modèle de la SkyBox
    SkyBox skyBox;
    Mesh meshSkyBox;
    meshSkyBox = CubeDerriere();
    meshSkyBox.setShaders(shadersTex);
    //meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    meshSkyBox = CubeGauche();
    meshSkyBox.setShaders(shadersTex);
    meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    meshSkyBox = CubeDevant();
    meshSkyBox.setShaders(shadersTex);
    meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    meshSkyBox = CubeDroite();
    meshSkyBox.setShaders(shadersTex);
    meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    meshSkyBox = CubeHaut();
    meshSkyBox.setShaders(shadersTex);
    meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    meshSkyBox = CubeBas();
    meshSkyBox.setShaders(shadersTex);
    meshSkyBox.setSkyBox();
    skyBox.addMesh(meshSkyBox);
    // Ajout de la SkyBox à la scène
    modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 10.0f));
    scene.addObject(skyBox, modelMatrix);
/*/
    // Surface de la pyramide "mur"
    Pyramide meshPyramide;
    meshPyramide.setShaders(shadersTex) ;
    // Modèle de la pyramide
    Model pyramide;
    pyramide.addMesh(meshPyramide);
    // Ajout de la pyramide à la scène
    modelMatrix = glm::translate(glm::mat4(1), glm::vec3(2.0f, 0.0f, -2.0f));
    scene.addObject(pyramide, modelMatrix);

    // Surface de la pyramide colorée
    PyramideCouleur meshPyramideCouleur;
    meshPyramideCouleur.setShaders(shaders) ;
    // Modèle de la pyramide
    RigidModel pyramideCouleur;
    pyramideCouleur.addMesh(meshPyramideCouleur);
    // Ajout de la pyramide à la scène
    modelMatrix = glm::mat4(1);
    scene.addObject(pyramideCouleur, modelMatrix);
    // Ajout au moteur physique
    pyramideCouleur.setBoxBody(1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f);
    scene.getDynamicsWolrd_ptr()->addRigidBody( pyramideCouleur.getRigidBody_ptr() );

    // Cube coloré
    CubeCouleur meshCubeCouleur;
    meshCubeCouleur.setShaders(shaders);
    // Modèle
    RigidModel cubeCouleur;
    cubeCouleur.addMesh(meshCubeCouleur);
    // Ajout à la scène
    modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f,10.0f,0.0f));
    scene.addObject(cubeCouleur,modelMatrix);
    // Ajout au moteur physique
    cubeCouleur.setBoxBody(1.0f,1.0f,1.0f,0.0f,10.0f,0.0f,10.0f);
    scene.getDynamicsWolrd_ptr()->addRigidBody( cubeCouleur.getRigidBody_ptr() );

    // Modèle et surface de Suzanne
    Model suzanne;
    suzanne.loadSurface("../rsc/Suzanne.obj", shadersSuz);
    // Ajout de Suzanne à la scène
    modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.0f, 0.0f, 2.0f));
    scene.addObject(suzanne, modelMatrix);

    // Configuration de la caméra
    OpglTerm::context.getCamera().setProjection
        ( glm::perspective( glm::radians(45.0f), 1.0f, 0.1f, 100.0f ) );
    OpglTerm::context.getCamera().setView
        ( glm::lookAt( glm::vec3(0,0,30), glm::vec3(0,0,0), glm::vec3(0,1,0) ) );
    
    // Lance l'interaction avec l'utilisateur
    OpglTerm::context.runInteraction();

    return EXIT_SUCCESS;
}