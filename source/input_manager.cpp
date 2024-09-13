#include "input_manager.h"

namespace depressing
{
  input_manager::input_manager ()
    : _prev_keys          {},
      _curr_keys          {},
      _prev_mouse_buttons {},
      _curr_mouse_buttons {},
      _mouse_pos          {glm::vec2 {0.f}},
      _is_mouse_moving    {false}
  {
  }

  void
  input_manager::begin_frame ()
  {
    _is_mouse_moving = false;

    for (u32 i{}; i < _prev_keys.size(); ++i)
      _prev_keys[i] = _curr_keys[i];

    for (u32 i{}; i < _prev_mouse_buttons.size(); ++i)
      _prev_mouse_buttons[i] = _curr_mouse_buttons[i];
  }
};
