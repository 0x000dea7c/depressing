#include "SDL.h"
#include "depressing.h"
#include "types.h"
#include "glad/glad.h"

#include "input_manager.cpp"
#include "game.cpp"

#include <glm/glm.hpp>
#include <print>
#include <memory>
#include <chrono>

namespace depressing
{
  class SDL_subsystem;
  class window;
};

// These functions go here because they are dependent on a library (SDL). If you ever need to change the
// library you're using for input handling, the idea is to only modify this file.
static depressing::key_codes
SDL_key_to_engine (i32 sdl_key)
{
  switch (sdl_key)
    {
    case SDLK_q:
      return depressing::key_codes::q;
    case SDLK_w:
      return depressing::key_codes::w;
    case SDLK_e:
      return depressing::key_codes::e;
    case SDLK_r:
      return depressing::key_codes::r;
    case SDLK_t:
      return depressing::key_codes::t;
    case SDLK_y:
      return depressing::key_codes::y;
    case SDLK_u:
      return depressing::key_codes::u;
    case SDLK_i:
      return depressing::key_codes::i;
    case SDLK_o:
      return depressing::key_codes::o;
    case SDLK_p:
      return depressing::key_codes::p;
    case SDLK_a:
      return depressing::key_codes::a;
    case SDLK_s:
      return depressing::key_codes::s;
    case SDLK_d:
      return depressing::key_codes::d;
    case SDLK_f:
      return depressing::key_codes::f;
    case SDLK_g:
      return depressing::key_codes::g;
    case SDLK_h:
      return depressing::key_codes::h;
    case SDLK_j:
      return depressing::key_codes::j;
    case SDLK_k:
      return depressing::key_codes::k;
    case SDLK_l:
      return depressing::key_codes::l;
    case SDLK_z:
      return depressing::key_codes::z;
    case SDLK_x:
      return depressing::key_codes::x;
    case SDLK_c:
      return depressing::key_codes::c;
    case SDLK_v:
      return depressing::key_codes::v;
    case SDLK_b:
      return depressing::key_codes::b;
    case SDLK_n:
      return depressing::key_codes::n;
    case SDLK_m:
      return depressing::key_codes::m;
    case SDLK_ESCAPE:
      return depressing::key_codes::esc;
    default:
      return depressing::key_codes::not_handled;
    }
}

static depressing::mouse_buttons
SDL_mouse_button_to_engine (i32 sdl_mouse_button)
{
  switch (sdl_mouse_button)
    {
    case SDL_BUTTON_LEFT:
      return depressing::mouse_buttons::left;
    case SDL_BUTTON_RIGHT:
      return depressing::mouse_buttons::right;
    default:
      return depressing::mouse_buttons::not_handled;
    }
}

// The only global variables that you'll have in this pathetic game. You don't need to worry about
// the order of initialisation because you need to call their init () functions. Naturally, you first
// want to initialise SDL's subsystem and then the window.
std::unique_ptr<depressing::SDL_subsystem> g_game_subsystem;
std::unique_ptr<depressing::window>        g_game_window;
std::unique_ptr<depressing::input_manager> g_game_input_manager;
std::unique_ptr<depressing::game>          g_game;

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

    b32
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

      SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL_major_version);
      SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, OpenGL_minor_version);
      SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, OpenGL_double_buffer);
      SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, OpenGL_depth_size);

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
    if (!g_game_window->init ())
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
    if (!g_game_window->set_title (title))
      return false;

    return true;
  }

  void
  game_shutdown ()
  {
    g_game->shutdown ();
  }

  b32
  game_is_shutting_down ()
  {
    return g_game->is_shutting_down ();
  }

  void
  game_run ()
  {
    using namespace depressing;
    using namespace std::chrono;

    auto previous_frame = high_resolution_clock::now ();
    f32 lag {0.0f};

    while (! g_game->is_shutting_down ())
      {
	SDL_Event event;

	while (SDL_PollEvent (&event))
	  {
	    b32 user_forces_shutdown {
	      (event.type == SDL_QUIT) ||
	      (event.type == SDL_WINDOWEVENT &&
	       event.window.event == SDL_WINDOWEVENT_CLOSE &&
	       event.window.windowID == SDL_GetWindowID(g_game_window->get_window()))
	    };

	    if (user_forces_shutdown)
	      {
		g_game->shutdown ();
		break;
	      }

	    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	      {
		key_codes k {SDL_key_to_engine (event.key.keysym.sym)};
		b32 pressed {event.type == SDL_KEYDOWN};
		g_game_input_manager->update_key(k, pressed);
	      }
	    else if (event.type == SDL_MOUSEMOTION)
	      {
		g_game_input_manager->update_mouse_position (glm::vec2{event.motion.xrel, -event.motion.yrel});
		g_game_input_manager->mouse_is_moving_now ();
	      }
	    else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	      {
		mouse_buttons b {SDL_mouse_button_to_engine (event.button.button)};
		b32 pressed {event.type == SDL_MOUSEBUTTONDOWN};
		g_game_input_manager->update_mouse_button (b, pressed);
	      }
	  }

	if (g_game->is_shutting_down ())
	  continue;

	auto current_frame = high_resolution_clock::now ();
	auto time_elapsed  = duration<f32> ((current_frame - previous_frame)).count ();
	previous_frame     = current_frame;
	lag               += time_elapsed;

	while (lag >= g_game->get_fixed_timestep ())
	  {
	    g_game->update ();
	    lag -= g_game->get_fixed_timestep ();
	  }

	// Value between [0, 1] representing how far you are between the last two physics computations.
	f32 interpolation {lag / g_game->get_fixed_timestep ()};

	g_game->render (interpolation);

	SDL_GL_SwapWindow (g_game_window->get_window ());
      }
  }
};

int
main ()
{
  using namespace depressing;

  // Pointer initialisation.
  g_game_subsystem = std::make_unique<SDL_subsystem> ();

  if (!g_game_subsystem)
    return EXIT_FAILURE;

  g_game_window = std::make_unique<window> ();

  if (!g_game_window)
    return EXIT_FAILURE;

  g_game_input_manager = std::make_unique<input_manager> ();

  if (!g_game_input_manager)
    return EXIT_FAILURE;

  g_game = std::make_unique<game> ();

  if (!g_game)
    return EXIT_FAILURE;

  if (!g_game_subsystem->init ())
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
      std::print("{0} - Couldn't create SDL window: {1}\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  // <XXX: This part is a hack to resize window to native desktop resolution. Figure this crap out.
  i32 display_index {SDL_GetWindowDisplayIndex (_window)};

  if (display_index < 0)
    {
      std::print("{0} - Couldn't get display index info: {1}\n", __FUNCTION__, SDL_GetError());
      return false;
    }

  if (SDL_GetDesktopDisplayMode (display_index, &display_mode) != 0)
    {
      std::print("{0} - Couldn't get desktop display mode: {1}\n", __FUNCTION__, SDL_GetError());
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
