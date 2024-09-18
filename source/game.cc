#include "game.hh"
#include "player.hh"

#include <print>

namespace depressing
{
  game::game ()
    : _player         {},
      _fixed_timestep {1.0f / 60.0f}, // 60 updates per second
      _quit           {false}
  {
  }

  b32
  game::init (std::shared_ptr<input_manager> input_manager)
  {
    _player = std::make_unique<player>(input_manager, this);

    if (!_player)
      {
	std::print("{0}: Couldn't create player pointer, out of memory.", __PRETTY_FUNCTION__);
	return false;
      }

    return true;
  }

  void
  game::process_player_input ()
  {
    _player->process_input ();
  }

  void
  game::update ()
  {
    _player->update ();
  }

  void
  game::render ([[maybe_unused]]f32 interpolation)
  {
    glClearColor (0.f, 0.f, 0.f, 1.f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render player sprite, get correct animation
  }

};
