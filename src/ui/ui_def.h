#include <format>
#include "def.h"

#ifndef __ui_def_h_
#define __ui_def_h_

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

  /* Mask class */
  class mask
  {
  private:

    ivec2 Pos0, Pos1;
    
  public:

    /* Mask construct function */
    mask( VOID ) :
      Pos0({0, 0}),
      Pos1({0, 0})
    {
    } /* End of 'mask' function */

    /* Mask construct function */
    mask( const ivec2 &NewPos0, const ivec2 &NewPos1 ) :
      Pos0(NewPos0),
      Pos1(NewPos1)
    {
    } /* End of 'mask' function */

    /* Mask construct function */
    mask( const ivec2 &NewPos0, const isize2 &NewSize ) :
      Pos0(NewPos0),
      Pos1({Pos0.X + NewSize.W - 1, Pos0.Y + NewSize.H - 1})
    {
    } /* End of 'mask' function */

    /* Intersect point with mask function */
    BOOL operator()( const ivec2 &Point ) const
    {
      return
        Point.X > Pos0.X && Point.X < Pos1.X &&
        Point.Y > Pos0.Y && Point.Y < Pos1.Y;
    } /* End of 'operator()' function */

    /* Intersect line with mask function */
    BOOL operator()( INT Y, INT &X0, INT &X1 ) const
    {
      if (Y < Pos0.Y || Y > Pos1.Y)
        return false;

      X0 = std::max(X0, Pos0.X);
      X1 = std::min(X1, Pos1.X);
      return X0 <= X1;
    } /* End of 'operator()' function */

    BOOL IsExist( VOID ) const
    {
      return Pos1.Y >= Pos0.Y && Pos1.X >= Pos0.X;
    } /* End of 'isExist' function */

    mask Intersect( const mask &M ) const
    {
      return mask(ivec2(std::max(Pos0.X, M.Pos0.X), std::max(Pos0.Y, M.Pos0.Y)),
                  ivec2(std::min(Pos1.X, M.Pos1.X), std::min(Pos1.Y, M.Pos1.Y)));
    } /* End of 'Intersect' function */

  }; /* End of 'mask' class */

} /* end of 'ui' namespace */

#endif // __ui_def_h_