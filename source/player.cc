#include "player.hh"
#include "input_manager.hh"
#include "game.hh"

namespace depressing
{
  player::player (std::shared_ptr<input_manager> input_manager, game const* g)
    : _events         {},
      _position       {0.0f, 0.0f},
      _speed          {1.0f, 1.0f},
      _input_manager  {input_manager},
      _fixed_timestep {g->get_fixed_timestep ()}
  {
  }

  void
  player::process_input ()
  {
    // Since updates are done in a fixed interval of time and input processing is done at a variable
    // interval of time, you need to push these things into a queue for further processing.
    if (_input_manager->is_key_pressed (key_codes::a))
      _events.push(player_events::moving_left);

    if (_input_manager->is_key_pressed (key_codes::d))
      _events.push(player_events::moving_right);

    if (_input_manager->is_key_pressed (key_codes::space))
      _events.push(player_events::jumping);
  }

  void
  player::update ()
  {
    while (! _events.empty ())
      {
	auto event = _events.front ();

	process_event (event);

	_events.pop ();
      }
  }

  void
  player::process_event (player_events ev)
  {
    switch (ev)
      {
      case player_events::moving_left:
	move_left ();
	break;
      case player_events::moving_right:
	move_right ();
	break;
      case player_events::jumping:
	jump ();
	break;
      }
  }

  void
  player::move_left ()
  {
    _position.x -= _speed.x * _fixed_timestep;
  }

  void
  player::move_right ()
  {
    _position.x += _speed.x * _fixed_timestep;
  }

  void
  player::jump ()
  {
    _position.y += _speed.y * _fixed_timestep;
  }
};
