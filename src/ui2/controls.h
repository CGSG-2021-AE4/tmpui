#include <iostream>

#ifndef __controls_h_
#define __controls_h_

#include "entry.h"

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

    VOID OnDraw( VOID ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size,
        State > entry_state::def ? State == entry_state::active ? RGB(200, 50, 50) : RGB(50, 100, 50) : RGB(50, 200, 50),
        RGB(100, 200, 100), SelfDrawMask);
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

} /* end of 'ui' namespace */

#endif // __controls_h_