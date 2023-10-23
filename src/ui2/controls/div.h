#ifndef __ui_controls_div_h_
#define __ui_controls_div_h_

#include "../entry.h"

namespace ui
{
  namespace controls
  {
    struct div_props
    {
      vec3
        SpaceColor = {0.4},  // Color of div space
        BorderColor = {0.7}; // Color of div border
      FLT
        BorderW = 1,                   // Width of border (now only 1)
        BorderR = 0;                   // Radius of div corners (isn't used now)
    }; /* End of 'div_props' struct */

    /* Div class */
    class div : public entry
    {
      div_props Props;

    public:
  
      /* Contsructor function */
      div( const ivec2 &NewPos, const isize2 &NewSize, const div_props &NewProps, const std::vector<entry *> &NewChildren = {} ) :
        entry(NewPos, NewSize, NewChildren),
        Props(NewProps)
      {
      } /* End of 'div' function */

      /* Contsructor with id function */
      div( const std::string &NewId, const ivec2 &NewPos, const isize2 &NewSize, const div_props &NewProps, const std::vector<entry *> &NewChildren = {} ) :
        entry(NewId, NewPos, NewSize, NewChildren),
        Props(NewProps)
      {
      } /* End of 'div' function */

      /* Overrided get content mask function */
      mask GetContentMask( VOID ) override
      {
        return {GlobalPos + ivec2(Props.BorderW), Size - isize2(Props.BorderW * 2)};
      } /* End of 'GetContentMask' function */
  
      /* On click event function */
      VOID OnClick( const ivec2 &LocalMousePos ) override
      {
      } /* End of 'OnClick' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        const FLT Coef = State > entry_state::def ? 0.8 : 1; // Just for debug

        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Props.BorderColor), ToRGB(Props.SpaceColor * Coef), SelfDrawMask);
      } /* End of 'OnDraw' function */

      /* On hover event function */
      VOID OnHover( const ivec2 &LocalMousePos ) override
      {
        Draw();
      } /* End of 'OnHover' function */
      
      /* On unhover event function */
      VOID OnUnhover( const ivec2 &LocalMousePos ) override
      {
        Draw();
      } /* End of 'OnUnhover' function */

    }; /* End of 'div' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_div_h_