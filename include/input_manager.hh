#pragma once

#include "types.hh"
#include "key_codes.hh"
#include "mouse_buttons.hh"

#include <array>
#include <glm/glm.hpp>

namespace depressing
{
  class input_manager final
  {
  public:
     input_manager ();
    ~input_manager () = default;

    void begin_frame ();

    void
    update_key (key_codes k, b32 pressed)
    {
      _curr_keys[static_cast<u32>(k)] = pressed;
    }

    void
    update_mouse_button (mouse_buttons m, b32 pressed)
    {
      _curr_keys[static_cast<u32>(m)] = pressed;
    }

    b32
    is_key_pressed (key_codes k) const
    {
      return ! _prev_keys[static_cast<u32>(k)] && _curr_keys[static_cast<u32>(k)];
    }

    b32
    is_key_held (key_codes k) const
    {
      return _prev_keys[static_cast<u32>(k)] && _curr_keys[static_cast<u32>(k)];
    }

    b32
    is_key_released (key_codes k) const
    {
      return _prev_keys[static_cast<u32>(k)] && ! _curr_keys[static_cast<u32>(k)];
    }

    b32
    is_mouse_button_pressed (mouse_buttons m) const
    {
      return ! _prev_mouse_buttons[static_cast<u32>(m)] && _curr_mouse_buttons[static_cast<u32>(m)];
    }

    b32
    is_mouse_button_held (mouse_buttons m) const
    {
      return _prev_mouse_buttons[static_cast<u32>(m)] && _curr_mouse_buttons[static_cast<u32>(m)];
    }

    b32
    is_mouse_button_released (mouse_buttons m) const
    {
      return _prev_mouse_buttons[static_cast<u32>(m)] && ! _curr_mouse_buttons[static_cast<u32>(m)];
    }

    b32
    is_mouse_moving () const
    {
      return _is_mouse_moving;
    }

    void
    update_mouse_position (glm::vec2 const&& pos)
    {
      _mouse_pos = pos;
    }

    void
    mouse_is_moving_now ()
    {
      _is_mouse_moving = true;
    }

  private:
    std::array<b32, static_cast<u32>(key_codes::count)> _prev_keys;
    std::array<b32, static_cast<u32>(key_codes::count)> _curr_keys;
    std::array<b32, static_cast<u32>(mouse_buttons::count)> _prev_mouse_buttons;
    std::array<b32, static_cast<u32>(mouse_buttons::count)> _curr_mouse_buttons;
    glm::vec2 _mouse_pos;
    b32 _is_mouse_moving;
  };
};
