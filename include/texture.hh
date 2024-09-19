#pragma once

namespace depressing
{
  struct texture final
  {
    texture (u32 OpenGL_id, i32 width, i32 height)
      : _OpenGL_id {OpenGL_id},
	_width     {width},
	_height    {height}
    {}

    u32 _OpenGL_id;
    i32 _width;
    i32 _height;
  };
};
