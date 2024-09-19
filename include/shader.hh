#pragma once

namespace depressing
{
  enum class shader_type {
    vertex,
    fragment,
    program
  };

  char const*
  get_shader_str (shader_type type)
  {
    switch (type)
      {
      case shader_type::vertex:
	return "vertex";
      case shader_type::fragment:
	return "fragment";
      case shader_type::program:
	return "program";
      }

    return "";
  }

  struct shader final
  {
    shader (u32 id, u32 vao, u32 vbo)
      : _id  {id},
	_vao {vao},
	_vbo {vbo}
    {}

    u32 _id;
    u32 _vao;
    u32 _vbo;
  };
};
