#include <string_view>

#ifndef __ui_controls_text_h_
#define __ui_controls_text_h_

#include "../entity.h"
#include "../frame_render.h"

namespace ui
{
  namespace controls
  {
    /* Text style structure */
    struct text_style
    {
      vec3 Color {0};
      DWORD LayoutFlags {0};
      BOOL IsEmptyBack {false};
      ivec2 Padding {2, 0};
    }; /* End of 'text_style' struct */

    /* Text props structure */
    struct text_props
    {
      BOOL IsSingleLine {false};
      std::string Str {""};
    }; /* End of 'div_props' struct */

    class text;

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::text>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {10};
      size_type MinSize {size_ref::eMinContent};
      size_type MaxSize {size_ref::eNone};
      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps { .MarginW = 4, .BorderW = 2, .PaddingW = 2 };

      controls::text_props Props {};
      controls::text_style Style {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Text class */
    class text : public entity
    {
      text_style Style;
      text_props Props;

    public:

      complex_text Str;
      
      /* Contsructor function */
      text( const entity_props<text> &NewProps ) :
        entity(NewProps),
        Props(NewProps.Props),
        Style(NewProps.Style)
      {
        IsBackgroundTransparent = Style.IsEmptyBack;
        if (Props.IsSingleLine)
          Str.Single(Props.Str);
        else
        {
          Str.Parse(Props.Str);
          Str.Wrap(InnerSize.W);
        }
      } /* End of 'text' function */

      isize2 GetMaxContent( VOID ) override
      {
        if (Props.IsSingleLine)
          return {(INT)Str.WholeStr.size() * render_2d::FontW + Style.Padding.X, render_2d::FontH};
        return {(INT)Str.MaxLineLen * render_2d::FontW + Style.Padding.X, render_2d::FontH};
      } /* End of 'UpdateMinMaxContent' function */

      isize2 GetMinContent( VOID ) override
      {
        return {(INT)Str.MaxWordLen * render_2d::FontW + Style.Padding.X, render_2d::FontH};
      } /* End of 'UpdateMinMaxContent' function */

      VOID OnResize( VOID ) override
      {
        if (!Props.IsSingleLine)
          Str.Wrap(InnerSize.W);
      } /* End of 'OnResize' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        if (!Style.IsEmptyBack)
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(vec3(0.8)), ToRGB(vec3(0.6)), BoxProps.BorderW, SelfDrawMask);

        if (Props.IsSingleLine)
          Canvas->Render2d.PutStr(Str.WholeStr, GlobalContentPos, InnerSize, Style.Padding, Style.LayoutFlags, 0, SelfDrawMask);
        else
          for (INT Line = 0; Line < (INT)Str.WrappedLines.size(); Line++)
            Canvas->Render2d.PutStr(Str.WrappedLines[Line], GlobalContentPos + ivec2(0, Line * render_2d::FontH), InnerSize, Style.Padding, Style.LayoutFlags, 0, SelfDrawMask);
      } /* End of 'OnDraw' function */

      /* Set text function */
      VOID SetStr( const std::string &NewStr )
      {
        Str.Parse(NewStr);
        Str.Wrap(InnerSize.W);
        OnUpdateContent();
        Redraw();
      } /* End of 'SetStr' function */

    }; /* End of 'text' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_text_h_