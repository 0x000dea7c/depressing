#version 460 core

in vec2 texture_coordinates;

out vec4 fragment_colour;

uniform sampler2D image;

void
main ()
{
  fragment_colour = texture (image, texture_coordinates);
}
