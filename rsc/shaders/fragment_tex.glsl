#version 430

in vec2 vertex_texture;
out vec4 fragment_color;

// Accès à l'unité de texture OpenGL active
uniform sampler2D tex_unit;

void main(void) { fragment_color = texture(tex_unit, vertex_texture); }
