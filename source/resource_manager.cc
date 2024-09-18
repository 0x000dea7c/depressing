#include "resource_manager.hh"

#include <SDL2/SDL_image.h>
#include <print>
#include <array>
#include <fstream>
#include <sstream>

namespace depressing
{
  resource_manager::resource_manager ()
    : _textures {}
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
	std::print ("{0}: couldn't load image surface for type {1}", __PRETTY_FUNCTION__, get_type_str (type));
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
    u32 shader_id {compile_link_shader ("./shaders/basic.vert", "./shaders/basic.frag")};

    if (shader_id == 0)
      return false;

    return true;
  }

  u32
  resource_manager::compile_link_shader (char const* vertex_filename, char const* fragment_filename)
  {
    // TODO: here!
    std::ifstream vertfs (vertex);
    std::ifstream fragfs (fragment);

    if (!vertfs) {
      std::cerr << __FUNCTION__ << ": couldn't open vertex file: " << vertex << '\n';
      return false;
    }

    if (!fragfs) {
      std::cerr << __FUNCTION__ << ": couldn't open fragment file: " << fragment << '\n';
      return false;
    }

    std::stringstream vertss;
    std::stringstream fragss;
    vertss << vertfs.rdbuf ();
    fragss << fragfs.rdbuf ();

    std::string vertcode {vertss.str()};
    std::string fragcode {fragss.str()};

    char const* vertcodec {vertcode.c_str ()};
    char const* fragcodec {fragcode.c_str ()};

    u32 vertexShaderId {glCreateShader (GL_VERTEX_SHADER)};

    glShaderSource (vertexShaderId, 1, &vertcodec, nullptr);
    glCompileShader (vertexShaderId);

    if (ShaderHasCompilationErrors(vertexShaderId, shader_type::vertex)) {
      glDeleteShader(vertexShaderId);
      return 0;
    }

    u32 fragmentShaderId{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderId, 1, &fragcodec, nullptr);
    glCompileShader(fragmentShaderId);
    if (ShaderHasCompilationErrors(fragmentShaderId, shader_type::fragment)) {
      glDeleteShader(vertexShaderId);
      glDeleteShader(fragmentShaderId);
      return 0;
    }

    u32 shaderProgram{glCreateProgram()};
    glAttachShader(shaderProgram, vertexShaderId);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);
    if (ShaderHasCompilationErrors(shaderProgram, shader_type::program)) {
      glDeleteShader(vertexShaderId);
      glDeleteShader(fragmentShaderId);
      return 0;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return shaderProgram;
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
};
