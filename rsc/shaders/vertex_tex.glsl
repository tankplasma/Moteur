#version 430

// Attributs VBO
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texture;

// Variable partagée
uniform mat4 mvp_matrix;

// Variables résultats (en plus de "gl_Position")
out vec2 vertex_texture;

void main(void) {
    gl_Position = mvp_matrix * vec4(position, 1.0);
    vertex_texture = texture;
}