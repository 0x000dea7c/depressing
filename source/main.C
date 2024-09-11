#include "SDL.h"
#include "glm.hpp"
#include "glad.h"
#include "depressing.H"

#include <print>

namespace depressing
{
  // Utility class for initialising and destroying SDL related resources.
  class SDL_subsystem
  {
  public:
    SDL_subsystem() = default;

    bool
    init()
    {
      int constexpr window_flags         = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER;
      int constexpr OpenGL_major_version = 4;
      int constexpr OpenGL_minor_version = 6;
      int constexpr OpenGL_double_buffer = 1; // yes
      int constexpr OpenGL_depth_size    = 24;

      if (SDL_Init(window_flags) < 0)
	{
	  std::print("{0} - Couldn't initialise SDL: {1}\n", __FUNCTION__, SDL_GetError());
	  return false;
	}

      if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
	  std::print("{0} - failed to initialise OpenGL function pointers\n", __FUNCTION__);
	  return false;
	}

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL_major_version);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGL_minor_version);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, OpenGL_double_buffer);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, OpenGL_depth_size);

      return true;
    }

    ~SDL_subsystem()
    {
      SDL_Quit();
    }
  };

  class window;

  class window
  {
  public:
     window();
    ~window();

    bool init();
    void set_title(char const* title);
  private:
    SDL_Window* _window;
    SDL_GLContext _context;
    int _flags;
    int _width;
    int _height;
    bool _fullscreen;
  };

  void
  window_toggle_fullscreen()
  {
    // TODO:
  }
};

int
main()
{
  using namespace depressing;

  SDL_subsystem subsystem;
  window game_window;

  if (!subsystem.init())
    return EXIT_FAILURE;

  if (!game_window.init())
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

using namespace depressing;

window::window()
  : _window{nullptr},
    _flags{SDL_WINDOW_OPENGL},
    _width{1920},
    _height{1080},
    _fullscreen{false}
{

}

window::~window()
{
  SDL_GL_DeleteContext(_context);
  SDL_DestroyWindow(_window);
}

bool
window::init()
{
  SDL_DisplayMode display_mode;

  SDL_GetDesktopDisplayMode(0, &display_mode);

  _width = display_mode.w;
  _height = display_mode.h;

  _window = SDL_CreateWindow("Depressing Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, _flags);

  if (!_window)
    {
      std::print("{0} - Couldn't create SDL window: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  // XXX: Hack to resize window to native desktop resolution.
  auto display_index = SDL_GetWindowDisplayIndex(_window);

  if (display_index < 0)
    {
      std::print("{0} - Couldn't get display index info: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  if (SDL_GetDesktopDisplayMode(display_index, &display_mode) != 0)
    {
      std::print("{0} - Couldn't get desktop display mode: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  _width = display_mode.w;
  _height = display_mode.h;

  SDL_SetWindowSize(_window, _width, _height);

  _context = SDL_GL_CreateContext(_window);

  SDL_GL_MakeCurrent(_window, _context);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return true;
}


void
window::set_title(char const* title)
{
  SDL_SetWindowTitle(_window, title);
}
