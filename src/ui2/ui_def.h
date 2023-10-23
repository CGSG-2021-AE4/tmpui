#include <format>
#include "def.h"

#ifndef __ui_def_h_
#define __ui_def_h_

//#include "../gfx/gfx_def.h"

#define DEBUG_LOG

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
#ifdef DEBUG_LOG
    std::cout << Str << "\n";
#endif // DEBUG_LOG
  } /* End of 'Log' function */

  /* Color ivec3 to RGB DWORD function.
   * ARGUMENTS:
   *   - color vector:
   *       const ivec3 &Color;
   * RETURNS:
   *   (DWORD) - RGB color.
   */
  template<typename Type>
    inline DWORD ToRGB( const mth::vec3<Type> &Color )
    {
      return RGB((UINT)(Color.X * 255), (UINT)(Color.Y * 255), (UINT)(Color.Z * 255));
    } /* End of 'ToRGB' function */

} /* end of 'ui' namespace */

#endif // __ui_def_h_