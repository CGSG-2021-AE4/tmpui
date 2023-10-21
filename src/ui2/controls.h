#include <iostream>
#include "entry.h"

#ifndef __controls_h_
#define __controls_h_


namespace ui
{
  /* Simple button control class */
  class button : public entry
  {
    BOOL IsClicked = false;
  
  public:
  
    button( const ivec2 &NewPos ) :
      entry(NewPos, {40, 20})
    {
    }
  
    VOID OnClick( const ivec2 &LocalMousePos ) override
    {
    }

    BOOL OnDraw( VOID ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size,
        State > entry_state::def ? State == entry_state::active ? RGB(200, 50, 50) : RGB(50, 100, 50) : RGB(50, 200, 50),
        RGB(100, 200, 100), SelfDrawMask);

      return 1;
    }

    VOID OnHover( const ivec2 &LocalMousePos ) override
    {
      Draw();
    }

    VOID OnUnhover( const ivec2 &LocalMousePos ) override
    {
      Draw();
    }

    VOID OnMouseDown( const ivec2 &LocalMousePos ) override
    {
      Draw();
    }

    VOID OnMouseUp( const ivec2 &LocalMousePos ) override 
    {
      Draw();
    }

  }; /* End of 'button' class */

  /* Div class */
  class div : public entry
  {
    ivec3 SpaceColor;

  public:
  
    div( const ivec2 &NewPos, const isize2 &NewSize, const ivec3 &NewSpaceColor, const std::vector<entry *> &NewChildren = {} ) :
      entry(NewPos, NewSize, NewChildren),
      SpaceColor(NewSpaceColor)
    {
    }

    mask GetContentMask( VOID ) override
    {
      return {GlobalPos + ivec2(1, 1), Size - isize2(2, 2)};
    }
  
    VOID OnClick( const ivec2 &LocalMousePos ) override
    {
    }

    BOOL OnDraw( VOID ) override
    {
      const FLT Coef = State > entry_state::def ? 0.5 : 1;

      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 255, 0), RGB(SpaceColor.X * Coef, SpaceColor.Y * Coef, SpaceColor.Z * Coef), SelfDrawMask);
      return 1;
    }

    VOID OnHover( const ivec2 &LocalMousePos ) override
    {
      Draw();
    }

    VOID OnUnhover( const ivec2 &LocalMousePos ) override
    {
      Draw();
    }

  }; /* End of 'button' class */

} /* end of 'ui' namespace */

#endif // __controls_h_