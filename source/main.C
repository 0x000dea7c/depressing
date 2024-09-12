#include "SDL.h"
#include "depressing.H"
#include "types.H"
#include "glad/glad.h"

#include <glm/glm.hpp>
#include <print>
#include <memory>

namespace depressing
{
  class SDL_subsystem;
  class window;
};

// The only global variables that you'll have in this pathetic game. You don't need to worry about
// the order of initialisation because you need to call their init () functions. Naturally, you first
// want to initialise SDL's subsystem and then the window.
std::unique_ptr<depressing::SDL_subsystem> subsystem;
std::unique_ptr<depressing::window>        game_window;
b32                                        quit {false};

namespace depressing
{
  // Utility class for initialising and destroying SDL window resources.
  class window
  {
  public:
     window (window const &) = delete;
     window ();
    ~window ();

    b32 init ();
    b32 set_title (char const* title);

    SDL_Window*
    get_window () const
    {
      return _window;
    }

    window& operator= (window const&) = delete;

  private:
    SDL_Window* _window;
    SDL_GLContext _context;
    i32 _flags;
    i32 _width;
    i32 _height;
    b32 _fullscreen;
  };

  // Utility class for initialising and destroying SDL related resources.
  class SDL_subsystem
  {
  public:
    SDL_subsystem () = default;

    bool
    init ()
    {
      i32 constexpr flags                {SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER};
      i32 constexpr OpenGL_major_version {4};
      i32 constexpr OpenGL_minor_version {6};
      i32 constexpr OpenGL_double_buffer {1}; // Means yes, use a double buffer.
      i32 constexpr OpenGL_depth_size    {24};

      if (SDL_Init (flags) < 0)
	{
	  std::print("{0} - Couldn't initialise SDL: {1}\n", __FUNCTION__, SDL_GetError());
	  return false;
	}

      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL_major_version);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGL_minor_version);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, OpenGL_double_buffer);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, OpenGL_depth_size);

      return true;
    }

    ~SDL_subsystem ()
    {
      SDL_Quit ();
    }
  };

  b32
  window_create ()
  {
    if (!game_window->init ())
      return false;

    return true;
  }

  b32
  window_toggle_fullscreen ()
  {
    // TODO: implement
    return true;
  }

  b32
  window_set_title (char const* title)
  {
    if (!game_window->set_title (title))
      return false;

    return true;
  }

  void
  game_shutdown ()
  {
    quit = true;
  }

  bool
  game_is_shutting_down ()
  {
    return quit;
  }

  void
  game_run ()
  {
    while (!game_is_shutting_down ())
      {
	SDL_Event event;

	while (SDL_PollEvent (&event))
	  {
	    b32 user_forces_shutdown {
	      event.type == SDL_QUIT ||
	      (event.type == SDL_WINDOWEVENT &&
	       event.window.event == SDL_WINDOWEVENT_CLOSE &&
	       event.window.windowID == SDL_GetWindowID(game_window->get_window()))
	    };

	    if (user_forces_shutdown)
	      game_shutdown();

	    // TODO: handle mouse and keyboard SDL events and store them in your structures
	  }

	if (game_is_shutting_down ())
	  continue;

	// TEMP
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(game_window->get_window());
      }
  }
};

int
main ()
{
  using namespace depressing;

  subsystem = std::make_unique<SDL_subsystem>();

  if (!subsystem)
    return EXIT_FAILURE;

  game_window = std::make_unique<window>();

  if (!game_window)
    return EXIT_FAILURE;

  if (!subsystem->init ())
    return EXIT_FAILURE;

  if (!window_create ())
    return EXIT_FAILURE;

  game_run ();

  return EXIT_SUCCESS;
}

depressing::window::window ()
  : _window     {nullptr},
    _context    {nullptr},
    _flags      {SDL_WINDOW_OPENGL},
    _width      {640},
    _height     {480},
    _fullscreen {false}
{

}

depressing::window::~window ()
{
  SDL_GL_DeleteContext (_context);
  SDL_DestroyWindow (_window);
}

b32
depressing::window::init ()
{
  SDL_DisplayMode display_mode;

  SDL_GetDesktopDisplayMode (0, &display_mode);

  _width  = display_mode.w;
  _height = display_mode.h;

  _window = SDL_CreateWindow ("Depressing Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, _flags);

  if (!_window)
    {
      std::print("{0} - Couldn't create SDL window: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  // <XXX: This part is a hack to resize window to native desktop resolution. Figure this crap out.
  i32 display_index {SDL_GetWindowDisplayIndex (_window)};

  if (display_index < 0)
    {
      std::print("{0} - Couldn't get display index info: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  if (SDL_GetDesktopDisplayMode (display_index, &display_mode) != 0)
    {
      std::print("{0} - Couldn't get desktop display mode: %s\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  _width  = display_mode.w;
  _height = display_mode.h;

  // >XXX

  SDL_SetWindowSize (_window, _width, _height);

  _context = SDL_GL_CreateContext (_window);

  SDL_GL_MakeCurrent (_window, _context);

  if (!gladLoadGLLoader ((GLADloadproc)SDL_GL_GetProcAddress))
    {
      std::print("{0} - failed to initialise OpenGL function pointers\n", __FUNCTION__);
      return false;
    }


  glEnable (GL_CULL_FACE);
  glEnable (GL_DEPTH_TEST);

  return true;
}

b32
depressing::window::set_title (char const* title)
{
  if (!title)
    return false;

  SDL_SetWindowTitle (_window, title);

  return true;
}
