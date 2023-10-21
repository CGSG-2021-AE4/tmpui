#ifndef __ui_def_h_
#define __ui_def_h_

#include "def.h"
#include <format>

//#include "../gfx/gfx_def.h"

namespace ui
{
  typedef tmp::vec4 vec4;
  typedef tmp::vec3 vec3;
  typedef tmp::vec2 vec2;

  typedef tmp::ivec3 ivec3;
  typedef tmp::ivec2 ivec2;

  typedef mth::size<INT> isize2;

  /* Simple log function */
  inline VOID Log( const std::string &Str )
  {
    std::cout << Str << "\n";
  } /* End of 'Log' function */

} /* end of 'ui' namespace */

#endif // __ui_def_h_