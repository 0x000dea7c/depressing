#version 460 core

layout (location = 0) in vec4 a_data;

out vec2 texture_coordinates;

void
main ()
{
  texture_coordinates = a_data.wz;

  gl_Position = vec4 (a_data.xy, 0.0, 1.0);
}
