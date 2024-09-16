#pragma once

namespace depressing
{
  enum class texture_type {
    player,
  };

  char const*
  get_filename_from_type (texture_type type)
  {
    switch (type)
      {
      case texture_type::player:
	return "./assets/player/warpsara-nohelmet-anim-sheet-alpha.png";
      }
  }
};
