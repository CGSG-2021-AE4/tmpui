#ifndef __controls_h_
#define __controls_h_

#include <iostream>
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
      std::cout << "Button was clicked" << std::endl;
    }

    VOID OnDraw( VOID ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 0, 255), RGB(100, 200, 100), DrawMask);
    }

    VOID OnHover( const ivec2 &LocalMousePos ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 255, 255), RGB(100, 0, 100), DrawMask);
      DrawChildren();
    }

    VOID OnUnhover( const ivec2 &LocalMousePos ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 0, 255), RGB(100, 200, 100), DrawMask);
      DrawChildren();
    }

  }; /* End of 'button' class */

  /* Div class */
  class div : public entry
  {
  public:
  
    div( const ivec2 &NewPos, const isize2 &NewSize, const std::vector<entry *> &NewChildren = {} ) :
      entry(NewPos, NewSize, NewChildren)
    {
    }
  
    VOID OnClick( const ivec2 &LocalMousePos ) override
    {
      std::cout << "Div was clicked" << std::endl;
    }

    VOID OnDraw( VOID ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 255, 0), RGB(50, 50, 50), DrawMask);
    }

    VOID OnHover( const ivec2 &LocalMousePos ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 255, 0), RGB(100, 100, 100), DrawMask);
      DrawChildren();
    }

    VOID OnUnhover( const ivec2 &LocalMousePos ) override
    {
      Canvas->Render2d.PutBar(GlobalPos, Size, RGB(0, 255, 0), RGB(50, 50, 50), DrawMask);
      DrawChildren();
    }

  }; /* End of 'button' class */

} /* end of 'ui' namespace */

#endif // __controls_h_