#include "resource_manager.hh"
#include "common.hh"

#include <SDL2/SDL_image.h>
#include <print>
#include <array>
#include <fstream>
#include <sstream>

namespace depressing
{
  resource_manager::resource_manager ()
    : _textures {},
      _shaders  {}
  {}

  b32
  resource_manager::init ()
  {
    for (u32 i {static_cast<u32> (texture_type::player)}; i < static_cast<u32> (texture_type::count); ++i)
      if (! load_texture (texture_type::player))
	return false;

    if (! load_shaders ())
      return false;

    return true;
  }

  b32
  resource_manager::load_texture (texture_type type)
  {
    SDL_Surface* image_surface {IMG_Load (get_filename_from_type (type))};

    if (!image_surface)
      {
	std::print ("{0}: couldn't load image surface for type {1}\n", __PRETTY_FUNCTION__, get_type_str (type));
	return false;
      }

    u32 OpenGL_texture_id;

    glGenTextures (1, &OpenGL_texture_id);
    glBindTexture (GL_TEXTURE_2D, OpenGL_texture_id);

    glPixelStorei (GL_UNPACK_ALIGNMENT, 1); // Don't assume dimensions are a multiple of 4. This avoids crashes.
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, image_surface->w, image_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_surface->pixels);
    glGenerateMipmap (GL_TEXTURE_2D);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture (GL_TEXTURE_2D, 0);

    _textures.emplace (type, texture {OpenGL_texture_id, image_surface->w, image_surface->h});

    SDL_FreeSurface (image_surface);

    return true;
  }

  b32
  resource_manager::load_shaders ()
  {
    u32 shader_id {compile_link_shader ("./shaders/main.vert", "./shaders/main.frag")};

    if (shader_id == 0)
      return false;

    create_quad_for_shader (shader_id, constants::shader_simple);

    return true;
  }

  u32
  resource_manager::compile_link_shader (char const* vertex_filename, char const* fragment_filename)
  {
    std::ifstream vertfs (vertex_filename);
    std::ifstream fragfs (fragment_filename);

    if (!vertfs)
      {
	std::print ("{0}: couldn't open vertex file at location: {1}\n", __PRETTY_FUNCTION__, vertex_filename);
	return false;
      }

    if (!fragfs)
      {
	std::print ("{0}: couldn't open fragment file at location: {1}\n", __PRETTY_FUNCTION__, fragment_filename);
	return false;
      }

    std::stringstream vertss;
    std::stringstream fragss;
    vertss << vertfs.rdbuf ();
    fragss << fragfs.rdbuf ();

    std::string vertcode {vertss.str ()};
    std::string fragcode {fragss.str ()};

    char const* vertcodec {vertcode.c_str ()};
    char const* fragcodec {fragcode.c_str ()};

    u32 vertex_shader_id {glCreateShader (GL_VERTEX_SHADER)};

    glShaderSource (vertex_shader_id, 1, &vertcodec, nullptr);
    glCompileShader (vertex_shader_id);

    if (shader_has_errors (vertex_shader_id, shader_type::vertex))
      {
	glDeleteShader (vertex_shader_id);
	return 0;
      }

    u32 fragment_shader_id {glCreateShader (GL_FRAGMENT_SHADER)};
    glShaderSource (fragment_shader_id, 1, &fragcodec, nullptr);
    glCompileShader (fragment_shader_id);

    if (shader_has_errors (fragment_shader_id, shader_type::fragment))
      {
	glDeleteShader (vertex_shader_id);
	glDeleteShader (fragment_shader_id);
	return 0;
      }

    u32 shader_program_id {glCreateProgram ()};
    glAttachShader (shader_program_id, vertex_shader_id);
    glAttachShader (shader_program_id, fragment_shader_id);
    glLinkProgram (shader_program_id);

    if (shader_has_errors (shader_program_id, shader_type::program))
      {
	glDeleteShader (vertex_shader_id);
	glDeleteShader (fragment_shader_id);
	return 0;
      }

    glDeleteShader (vertex_shader_id);
    glDeleteShader (fragment_shader_id);

    return shader_program_id;
  }

  b32
  resource_manager::shader_has_errors (u32 program_id, shader_type type) const
  {
    i32 success {0};
    std::array<char, 512> log {'\0'};

    switch (type)
      {
      case shader_type::vertex:
      case shader_type::fragment:
	{
	  glGetShaderiv (program_id, GL_COMPILE_STATUS, &success);

	  if (success != GL_TRUE)
	    {
	      glGetShaderInfoLog (program_id, log.size(), nullptr, log.data());
	      std::print ("{0} : couldn't compile {1} shader: {2}\n", __PRETTY_FUNCTION__, get_shader_str (type), log.data ());
	      return true;
	    }
	} break;
      case shader_type::program:
	{
	  glGetProgramiv (program_id, GL_LINK_STATUS, &success);

	  if (success != GL_TRUE)
	    {
	      glGetProgramInfoLog (program_id, log.size(), nullptr, log.data());
	      std::print ("{0} : couldn't link {1} shader: {2}\n", __PRETTY_FUNCTION__, get_shader_str (type), log.data ());
	      return true;
	    }
	} break;
      }

    return false;
  }

  void
  resource_manager::create_quad_for_shader (u32 shader_id, u32 shader_id_cache)
  {
    std::array<f32, 18> vertices
      {
	-1.f, -1.f, 0.f,
	 1.f, -1.f, 0.f,
	-1.f,  1.f, 0.f,
	-1.f,  1.f, 0.f,
	 1.f, -1.f, 0.f,
	 1.f,  1.f, 0.f,
      };

    u32 vao;
    u32 vbo;

    glGenVertexArrays (1, &vao);
    glGenBuffers (1, &vbo);
    glBindVertexArray (vao);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices.data (), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _shaders.emplace (shader_id_cache, shader {shader_id, vao, vbo});
  }
};
