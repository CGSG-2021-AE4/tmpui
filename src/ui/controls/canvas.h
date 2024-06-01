#ifndef __ui_controls_canvas_h_
#define __ui_controls_canvas_h_

#include "../entity.h"

#include "div.h"

namespace ui
{
  namespace controls
  {
    /* Canvas style structure */
    struct canvas_style
    {
      vec3
        BorderColor {0.75};
    }; /* End of 'canvas_style' struct */

    class canvas;

    /* Canvas props structure */
    struct canvas_props
    {
      std::function<VOID( const ivec2 &DrawPos, const isize2 &DrawSize, const mask &DrawMask )> OnDrawCallBack {[]( const ivec2 &DrawPos, const isize2 &DrawSize, const mask &DrawMask ){}};
      std::function<VOID( const ivec2 &Pos )> OnClickCallBack {[]( const ivec2 &Pos ){}};
    }; /* End of 'canvas_props' struct */
  } /* end of 'constrols' namespace */

  template<>
    struct entity_props<controls::canvas>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {0};
      size_type MinSize {size_ref::eNone};
      size_type MaxSize {size_ref::eNone};

      flex_props Flex {};
      controls::canvas_style Style {};
      controls::canvas_props Props {};
    };

  namespace controls
  {
    /* canvas class */
    class canvas : public entity
    {
      canvas_style Style;
      canvas_props Props;

    public:
  
      /* Contsructor function */
      canvas( const entity_props<canvas> &NewProps ) :
        entity(entity_props<div>{
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = layout_type::eFlexColumn,
          .Flex = NewProps.Flex,
          .BoxProps = { .BorderW = 2, },
          .Style = { .BorderColor = {0.3, 0.3, 0.75} },
        }),
        Style(NewProps.Style),
        Props(NewProps.Props)
      {
      } /* End of 'canvas' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        if (!IsBackgroundTransparent)
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.BorderColor), ToRGB(vec3(0)), BoxProps.BorderW, SelfDrawMask);

        Props.OnDrawCallBack(GlobalContentPos, InnerSize, ContentDrawMask);
      } /* End of 'OnDraw' function */

    }; /* End of 'canvas' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_canvas_h_