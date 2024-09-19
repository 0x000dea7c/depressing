#pragma once

#include "types.hh"

namespace depressing
{
  enum class texture_type : u32 {
    player,

    count
  };

  char const*
  get_filename_from_type (texture_type type)
  {
    switch (type)
      {
      case texture_type::player:
	return "./assets/player/warpsara-nohelmet-anim-sheet-alpha.png";
      case texture_type::count:
	break;
      }

    return "";
  }

  char const*
  get_type_str (texture_type type)
  {
    switch (type)
      {
      case texture_type::player:
	return "player";
      case texture_type::count:
	break;
      }

    return "";
  }
};
