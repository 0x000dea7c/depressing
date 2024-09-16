#pragma once

#include <glm/glm.hpp>
#include <queue>
#include <memory>
#include "player_events.h"

namespace depressing
{
  class input_manager;
  class game;

  class player final
  {
  public:
     player (std::shared_ptr<input_manager> input_manager, game const* g);
    ~player () = default;

    void process_input ();

    void update ();

  private:
    void process_event (player_events ev);
    void move_left ();
    void move_right ();
    void jump ();

    std::queue<player_events> _events;
    glm::vec2 _position;
    glm::vec2 const _speed;
    std::shared_ptr<input_manager> _input_manager; // Need to be sure this fucker exists while a reference of this class is alive.
    f32 const _fixed_timestep;
  };
};
