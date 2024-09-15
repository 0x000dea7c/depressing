 #include "game.h"

namespace depressing
{
  game::game ()
    : _fixed_timestep {1.0f / 60.0f}, // 60 updates per second
      _quit           {false}
  {
  }

  game::~game ()
  {
  }

  void
  game::update ()
  {
  }

  void
  game::render ([[maybe_unused]]f32 interpolation)
  {
    glClearColor (0.f, 0.f, 0.f, 1.f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

};
