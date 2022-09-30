#include "../inc/Mesh.hpp"

#include <stb_image.h>

#include <iostream>
using namespace std;

Mesh::Mesh()
: mTextureID(0) // Pas de texture par défaut
, mIsSkyBox(false)
{}

void Mesh::draw(glm::mat4& mvpContext) 
{
    // Active le programme GPU asscocié et les variables partagées
    glUseProgram(mShaders);
    // Demande à OpenGL un identifiant pour la variable partagée "mvp_matrix"
    GLuint mvpID = glGetUniformLocation(mShaders, "mvp_matrix");
    // Associe la variable GPU "mpv_matrix" avec la variable C++ "mvpContext"
    glUniformMatrix4fv(
        mvpID, 
        // Une seule matrice partagée
        1, 
        // Matrice partagée telle quelle, sans transposition
        GL_FALSE,
        // Pointeur vers la première case de la matrice partagée
        &mvpContext[0][0]
        );

    // Active éventuellement le z-buffer pour éliminer les faces cachées
    if (mIsSkyBox)
        glDisable(GL_DEPTH_TEST);
    else
        glEnable(GL_DEPTH_TEST);

    if (mTextureID != 0)
    {
        // Active l'unité de texture OpenGL n°0
        glActiveTexture(GL_TEXTURE0);
        // Indique à l'unité activée l'ID de la texture de référence et son type
        glBindTexture(GL_TEXTURE_2D, mTextureID);
    }

    // Dessine les triangles enregistrés dans le "Vertex Array Object"
    glBindVertexArray(mVAO);
    glDrawElements(
        // Mode d'affichage : triangle indépendants
        GL_TRIANGLES, 
        // Nombre de triangles à afficher
        /*static_cast<GLsizei>*/(mIndices.size()), 
        // Type des valeurs dans "mIndices" 
        GL_UNSIGNED_INT, 
        // si pas d'EBO : pointeur vers le buffer contenant les triangles
        nullptr
        );
    glBindVertexArray(0);

    glUseProgram(0);
}

void Mesh::setup(vector<Vertex> vertices, vector<GLuint> indices)
{
    mVertices = vertices;
    mIndices = indices;

    // Demande à OpenGL d'enregistrer et fournir 1 identifiant de VAO
    glGenVertexArrays(1, &mVAO);
    // Demande à OpenGL d'enregistrer et fournir 1 identifiant de buffer
    glGenBuffers(1, &mVBO);
    // Demande à OpenGL d'enregistrer et fournir 1 identifiant de buffer
    glGenBuffers(1, &mEBO);

    // Indique "mVAO" comme étant le "Vertex Array Object" actif
    glBindVertexArray(mVAO);

    // * TRIANGLES
    // Dans le contexte du VAO actif, associe "mEBO" comme liste de triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    // Associe l'identifiant mEBO activé avec les triangles de "mIndices"
    glBufferData(
        // Buffer de type "liste de triangles"
        GL_ELEMENT_ARRAY_BUFFER,
        // Taille en octets du buffer
        mIndices.size() * sizeof(GLuint),
        // Adresse du buffer = Adresse de la première case...
        &mIndices[0],
        // Type d'utilisation du buffer : affichage de données invariables
        GL_STATIC_DRAW
        );

    // * ATTRIBUTS
    // Dans le contexte du VAO actif, associe "mVBO" comme liste des attributs
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // Associe l'identifiant mVBO activé avec les coordonnée de "mVertex"
    glBufferData(
        // Buffer de type "liste d'attributs"
        GL_ARRAY_BUFFER,
        // Taille en octets du buffer
        mVertices.size() * sizeof(Vertex),
        // Adresse du buffer = Adresse de la première case...
        &mVertices[0],
        // Type d'utilisation du buffer : affichage de données invariables
        GL_STATIC_DRAW
        );

    // * Attribut 0 => POSITION x,y,z
    // Dans le contexte du VAO actif, active l'attribut 0 pour les positions
    glEnableVertexAttribArray(0);
    // Decrit la façon d'accéder aux données de l'attribut 0
    glVertexAttribPointer(
        // Numéro de l'attribut décrit
        0, 
        // Nombre de valeurs dans l'attribut
        3, 
        // Type des valeurs
        GL_FLOAT, 
        // Normalisation des valeurs sur [0,1[ (uniquement pour le type GL_FIXED)
        GL_FALSE, 
        // Décalage entre les positions successives de l'attribut 0 dans le buffer
        sizeof(Vertex), 
        // Position de la première occurence de l'attribut 0 dans le buffer
        (void*)offsetof(Vertex, position)
        );

    // * Attribut 1 => COULEUR r,g,b
    // Dans le contexte du VAO actif, active l'attribut 1 pour les couleurs
    glEnableVertexAttribArray(1);
    // Decrit la façon d'accéder aux données de l'attribut 1
    glVertexAttribPointer(
        // Numéro de l'attribut décrit
        1, 
        // Nombre de valeurs dans l'attribut : (r,g,b)
        3, 
        // Type des valeurs
        GL_FLOAT, 
        // Normalisation des valeurs sur [0,1[ (uniquement pour le type GL_FIXED)
        GL_FALSE, 
        // Décalage entre les positions successives de l'attribut 0 dans le buffer
        sizeof(Vertex), 
        // Position de la première occurence de l'attribut 0 dans le buffer
        (void*)offsetof(Vertex, color)
        );

    // * Attribut 2 => TEXTURE u,v
    // Dans le contexte du VAO actif, active l'attribut 2 pour la texture
    glEnableVertexAttribArray(2);
    // Decrit la façon d'accéder aux données de l'attribut 1
    glVertexAttribPointer(
        // Numéro de l'attribut décrit
        2, 
        // Nombre de valeurs dans l'attribut : (u,v)
        2, 
        // Type des valeurs
        GL_FLOAT, 
        // Normalisation des valeurs sur [0,1[ (uniquement pour le type GL_FIXED)
        GL_FALSE, 
        // Décalage entre les positions successives de l'attribut 0 dans le buffer
        sizeof(Vertex), 
        // Position de la première occurence de l'attribut 0 dans le buffer
        (void*)offsetof(Vertex, texture)
        );

    // Désactive "mVAO"
    glBindVertexArray(0);
}

void Mesh::setTextureID(GLuint textureID) { mTextureID = textureID; }

void Mesh::setShaders(GLuint shaders) { mShaders = shaders; }

void Mesh::setSkyBox() { mIsSkyBox = true; }

void Mesh::loadTexture(const string fileName)
{
    // Texture de la pyramide
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options 
    // (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load
        (fileName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        cout << "Chargement de la texture \"" << fileName << "\" OK" << endl;
    }
    else
    {
        cout << "Impossible de charger la texture \"" 
            << fileName << "\" !!!" << endl;
    }
    stbi_image_free(data);
    //
    this->setTextureID(textureID);
}

Pyramide::Pyramide()
{
    this->loadTexture("../rsc/bricks.jpg");

    // Sommets de la pyramide
    Vertex vertex;
    //
    vector<Vertex> vertices;
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f);
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f);
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f);
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f);
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 0.0f, 1.0f, 0.0f);
    vertex.texture = glm::vec2(0.5f, 1.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    //
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    //
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(4);
    //
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    //
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    //
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(4);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

PyramideCouleur::PyramideCouleur()
{
    // Sommets de la pyramide
    Vertex vertex;
    //
    vector<Vertex> vertices;
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f);
    vertex.color = glm::vec3(1.0f,0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f);
    vertex.color = glm::vec3(0.0f,1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f);
    vertex.color = glm::vec3(0.0f,0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f);
    vertex.color = glm::vec3(1.0f,1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 0.0f, 1.0f, 0.0f);
    vertex.color = glm::vec3(1.0f,1.0f, 1.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    //
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    //
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(4);
    //
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    //
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    //
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(4);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeDerriere::CubeDerriere()
{
    this->loadTexture("../rsc/skybox/back.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f); //2
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f); //3
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f,-1.0f); //6
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f,-1.0f); //7
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Derrière
    indices.push_back(1);//(3);
    indices.push_back(0);//(2);
    indices.push_back(2);//(6);
    //
    indices.push_back(1);//(3);
    indices.push_back(3);//(7);
    indices.push_back(2);//(6);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeGauche::CubeGauche()
{
    this->loadTexture("../rsc/skybox/right.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f); //0
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f); //3
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f, 1.0f); //4
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f,-1.0f); //7
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Gauche
    indices.push_back(3);//(7);
    indices.push_back(2);//(4);
    indices.push_back(1);//(3);
    //
    indices.push_back(2);//(4);
    indices.push_back(1);//(3);
    indices.push_back(0);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeDevant::CubeDevant()
{
    this->loadTexture("../rsc/skybox/front.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f); //0
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f); //1
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f, 1.0f); //4
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f, 1.0f); //5
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Devant
    indices.push_back(3);//(5);
    indices.push_back(2);//(4);
    indices.push_back(1);
    //
    indices.push_back(2);//(4);
    indices.push_back(1);
    indices.push_back(0);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeDroite::CubeDroite()
{
    this->loadTexture("../rsc/skybox/left.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f); //1
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f); //2
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f, 1.0f); //5
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f,-1.0f); //6
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Droite
    indices.push_back(2);//(5);
    indices.push_back(1);//(2);
    indices.push_back(3);//(6);
    //
    indices.push_back(2);//(5);
    indices.push_back(1);//(2);
    indices.push_back(0);//(1);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeHaut::CubeHaut()
{
    this->loadTexture("../rsc/skybox/top.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3(-1.0f,1.0f, 1.0f); //4
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f, 1.0f); //5
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f,-1.0f); //6
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f,-1.0f); //7
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Haut
    indices.push_back(0);//(4);
    indices.push_back(1);//(5);
    indices.push_back(2);//(6);
    //
    indices.push_back(0);//(4);
    indices.push_back(2);//(6);
    indices.push_back(3);// (7);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeBas::CubeBas()
{
    this->loadTexture("../rsc/skybox/bottom.jpg");

    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f);
    vertex.texture = glm::vec2(1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f);
    vertex.texture = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f);
    vertex.texture = glm::vec2(0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f);
    vertex.texture = glm::vec2(1.0f, 1.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Bas
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}

CubeCouleur::CubeCouleur()
{
    // Sommets
    Vertex vertex;
    vector<Vertex> vertices;
    //
    vertex.position = glm::vec3(-1.0f,-1.0f, 1.0f);
    vertex.color = glm::vec3(1.0f,0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f, 1.0f);
    vertex.color = glm::vec3(0.0f,1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,-1.0f,-1.0f);
    vertex.color = glm::vec3(0.0f,0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,-1.0f,-1.0f);
    vertex.color = glm::vec3(1.0f,1.0f, 0.0f);
    vertices.push_back(vertex);
    //
    vertex.position = glm::vec3(-1.0f,1.0f, 1.0f);
    vertex.color = glm::vec3(1.0f,0.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f, 1.0f);
    vertex.color = glm::vec3(0.0f,1.0f, 0.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3( 1.0f,1.0f,-1.0f);
    vertex.color = glm::vec3(0.0f,0.0f, 1.0f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-1.0f,1.0f,-1.0f);
    vertex.color = glm::vec3(1.0f,1.0f, 0.0f);
    vertices.push_back(vertex);

    // Triangles formant les côtés
    vector<GLuint> indices;
    // Bas
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    // Haut
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    //
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);
    // Derrière
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(6);
    //
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(6);
    // Droite
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);
    //
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(1);
    // Gauche
    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(3);
    //
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(0);
    // Devant
    indices.push_back(5);
    indices.push_back(4);
    indices.push_back(1);
    //
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(0);

    // Enregistement de la forme construite
    this->setup(vertices, indices);
}
