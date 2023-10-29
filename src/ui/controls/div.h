#ifndef __ui_controls_div_h_
#define __ui_controls_div_h_

#include "../entity.h"

namespace ui
{
  namespace controls
  {
    /* Div style structure */
    struct div_style
    {
      vec3
        SpaceColor = {0},  // Color of div space
        BorderColor = {1}; // Color of div border
    }; /* End of 'div_style' struct */

    /* Div style structure */
    struct div_props
    {
      // Entity part
      std::string Id {"Div"};
      ivec2 Pos {0};
      isize2 Size {0};
      layout_props LayoutProps {}; // Layout props
      box_props BoxProps {};

      // Div part
      div_style Style {};       // Div box style
    }; /* End of 'div_props' struct */

    /* Div class */
    class div : public entity
    {
      div_style Style;

    public:
  
      /* Contsructor function */
      div( const div_props &NewProps, const std::vector<entity *> &NewChildren = {} ) :
        entity(NewProps, NewChildren),
        Style(NewProps.Style)
      {
      } /* End of 'div' function */
      
      /* On click event function */
      BOOL OnClick( const ivec2 &LocalMousePos ) override
      {
        return true;
      } /* End of 'OnClick' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        const FLT Coef = State > entity_state::eDef ? 0.8 : 1; // Just for debug

        if (BoxProps.BorderW)
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.BorderColor), ToRGB(Style.SpaceColor * Coef), BoxProps.BorderW, SelfDrawMask);
        else
          Canvas->Render2d.FillBar(GlobalPos, Size, ToRGB(Style.SpaceColor * Coef), SelfDrawMask);
      } /* End of 'OnDraw' function */

      /* On hover event function */
      BOOL OnHover( const ivec2 &LocalMousePos ) override
      {
        return true;
      } /* End of 'OnHover' function */
      
      /* On unhover event function */
      BOOL OnUnhover( const ivec2 &LocalMousePos ) override
      {
        return true;
      } /* End of 'OnUnhover' function */

    }; /* End of 'div' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_div_h_