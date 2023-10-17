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
  }; /* End of 'button' class */
} /* end of 'ui' namespace */

#endif // __controls_h_