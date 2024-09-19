#pragma once

#include "types.hh"

namespace depressing
{
  // TODO: This is not a good idea but I'm stupid and can't figure out a better way to do it.
  // The idea is to avoid having to index shaders by strings and have handles instead to the
  // shaders that you want to use... I don't want to have a runtime error because I miss typed
  // the id of the shader...
  constexpr u32
  string_length (char const* str)
  {
    u32 length {0};

    while (*str++)
      ++length;

    return length;
  }

  constexpr i32
  fnv1a (char const* str, u32 length) // Wikipedia.
  {
    i32 constexpr prime {0x01000193}; // FNV-1a 32-bit prime.
    u32 hash {0x811c9dc5};   // FNV-1a 32-bit offset basis.

    for (u32 i {}; i < length; ++i)
      {
	hash ^= static_cast<u32> (str[i]);
	hash *= prime;
      }

    return hash;
  }

  namespace constants
  {
    i32 constexpr shader_simple {fnv1a ("simple", string_length ("simple"))};
  };

};
