#pragma once

#include "types.h"

namespace depressing
{
  class game final
  {
  public:
     game ();
    ~game ();

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

    void update ();

    void render (f32 interpolation);

  private:
    f32 _fixed_timestep;
    b32 _quit;
  };
};
