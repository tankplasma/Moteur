#version 430

// Attributs VBO
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// Variable partagée
uniform mat4 mvp_matrix;

// Variables résultats (en plus de "gl_Position")
out vec3 vertex_color;

void main(void) {
    gl_Position = mvp_matrix * vec4(position, 1.0);
    vertex_color = color;
}