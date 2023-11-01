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
        SpaceColor,
        BorderColor;
    }; /* End of 'div_style' struct */

    /* Div class */
    class div : public entity
    {
      div_style Style;

    public:
  
      /* Contsructor function */
      div( const entity_props<BYTE, div_style> &NewProps, const std::vector<entity *> &NewChildren = {} ) :
        entity(NewProps, NewChildren),
        Style(NewProps.Style)
      {
      } /* End of 'div' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.BorderColor), ToRGB(Style.SpaceColor), BoxProps.BorderW, SelfDrawMask);
      } /* End of 'OnDraw' function */

    }; /* End of 'div' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_div_h_