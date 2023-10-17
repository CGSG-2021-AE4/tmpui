#ifndef __ui_def_h_
#define __ui_def_h_

#include "def.h"

#include "../gfx/gfx_def.h"
namespace ui
{

  typedef tmp::vec4 vec4;
  typedef tmp::vec3 vec3;
  typedef tmp::vec2 vec2;

  typedef tmp::ivec3 ivec3;
  typedef tmp::ivec2 ivec2;

  typedef mth::size<INT> isize2;
  typedef ::tmp::gfx renderer;


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
      Pos1({Pos0.X + NewSize.W, Pos0.Y + NewSize.H})
    {
    } /* End of 'mask' function */

    BOOL operator()( const ivec2 &Point )
    {
      return
        Point.X >= Pos0.X && Point.X <= Pos1.X &&
        Point.Y >= Pos0.Y && Point.Y <= Pos1.Y;
    } /* End of 'operator()' function */

    mask Intersect( const mask &M )
    {
      return mask(ivec2(std::max(Pos0.X, M.Pos0.X), std::max(Pos0.Y, M.Pos0.Y)),
                  ivec2(std::min(Pos1.X, M.Pos1.X), std::min(Pos1.Y, M.Pos1.Y)));
    } /* End of 'Intersect' function */

  }; /* End of 'mask' class */

} /* end of 'ui' namespace */

#endif // __ui_def_h_