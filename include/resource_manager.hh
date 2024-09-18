#pragma once

#include "texture_type.hh"
#include "texture.hh"
#include "shader.hh"

#include <unordered_map>

namespace depressing
{
  class resource_manager final
  {
  public:
     resource_manager ();
    ~resource_manager () = default;

    b32 init ();

  private:
    b32 load_texture (texture_type type);

    b32 load_shaders ();

    u32 compile_link_shader (char const* vertex_filename, char const* fragment_filename);

    b32 shader_has_errors (u32 program_id, shader_type type) const;

    std::unordered_map<texture_type, texture> _textures;
  };
};
