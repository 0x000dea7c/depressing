#pragma once

#include "types.h"

namespace depressing
{
  // Prototypes for platform dependent code.

  b32  window_create ();
  b32  window_toggle_fullscreen ();
  b32  window_set_title (char const* title);

  void game_run ();
  void game_shutdown ();
  b32  game_is_shutting_down ();
};
