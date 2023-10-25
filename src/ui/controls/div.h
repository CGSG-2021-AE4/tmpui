#ifndef __ui_controls_div_h_
#define __ui_controls_div_h_

#include "../entry.h"

namespace ui
{
  namespace controls
  {

    /* Box style values struct */
    struct box_style
    {
      vec3
        SpaceColor = {0},  // Color of div space
        BorderColor = {1}; // Color of div border
      FLT
        BorderW = 1,       // Width of border (now only 1)
        BorderR = 0;       // Radius of div corners (isn't used now)
    }; /* End of 'box_style' struct */

    struct div_props
    {
      std::string Id {"Div"};
      ivec2 Pos {0};
      isize2 Size {0};
      layout_props LayoutProps {}; // Layout props
      box_style BoxStyle {};       // Div box style
    }; /* End of 'div_props' struct */

    /* Div class */
    class div : public entry
    {
      box_style BoxStyle;

    public:
  
      /* Contsructor function */
      div( const div_props &NewProps, const std::vector<entry *> &NewChildren = {} ) :
        entry(NewProps, NewChildren),
        BoxStyle(NewProps.BoxStyle)
      {
      } /* End of 'div' function */
      
      /* Overrided get content mask function */
      mask GetContentMask( VOID ) override
      {
        return {GlobalPos + ivec2(BoxStyle.BorderW), Size - isize2(BoxStyle.BorderW * 2)};
      } /* End of 'GetContentMask' function */
  
      /* On click event function */
      VOID OnClick( const ivec2 &LocalMousePos ) override
      {
      } /* End of 'OnClick' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        const FLT Coef = State > entry_state::eDef ? 0.8 : 1; // Just for debug

        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(BoxStyle.BorderColor), ToRGB(BoxStyle.SpaceColor * Coef), SelfDrawMask);
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