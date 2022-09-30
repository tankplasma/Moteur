#version 430
in vec3 vertex_color;
out vec3 fragment_color;
void main(void) { fragment_color = vertex_color; }
