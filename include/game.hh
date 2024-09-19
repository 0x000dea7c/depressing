#pragma once

#include "types.hh"

#include <memory>

namespace depressing
{
  class input_manager;
  class player;

  class game final
  {
  public:
     game ();
    ~game () = default;

    b32 init (std::shared_ptr<input_manager> input_manager);

    f32
    get_fixed_timestep () const
    {
      return _fixed_timestep;
    }

    void
    shutdown ()
    {
      _quit = true;
    }

    b32
    is_shutting_down () const
    {
      return _quit;
    }

    void process_player_input ();

    void update ();

    void render (f32 interpolation);

  private:
    std::unique_ptr<player> _player;
    f32 _fixed_timestep;
    b32 _quit;
  };
};
