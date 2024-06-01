#include <sstream>

#ifndef __ui_controls_slider_h_
#define __ui_controls_slider_h_

#include "../frame_render.h"
#include "../entity.h"

namespace ui
{
  namespace controls
  {
    /* Slider style values struct */
    struct slider_style
    {
      box_style
        Track, // Track style
        Thumb; // Thumb style
    }; /* End of 'slider_style' struct */

    /* Slider props struct */
    struct slider_props
    {
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
    }; /* End of 'slidre_props' struct */

    class slider;

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::slider>
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

      controls::slider_style Style {};
      controls::slider_props Props {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Slider class */
    class slider : public entity
    {
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
      slider_style Style {};

    public:
  
      /* Contsructor function */
      slider( const entity_props<slider> &NewProps ) :
        entity(NewProps),
        Style(NewProps.Style),
        Min(NewProps.Props.Min),
        Max(NewProps.Props.Max),
        Value(NewProps.Props.Value)
      {
      } /* End of 'slider' function */

      isize2 GetMinContent( VOID ) override
      {
        return {render_2d::FontW * 13, render_2d::FontH}; // Len of 'Value: *.***'
      } /* End of 'GetMaxContent' function */

      isize2 GetMaxContent( VOID ) override
      {
        return {render_2d::FontW * 13, render_2d::FontH}; // Len of 'Value: *.***'
      } /* End of 'GetMaxContent' function */

      VOID UpdateValue( const ivec2 &LocalMousePos )
      {
        FLT TrackL = BorderSize.W - BorderSize.H;
        Value = Min + ((FLT)LocalMousePos.X - BorderSize.H / 2) / TrackL * (Max - Min);
        Value = std::clamp(Value, Min, Max);
      }
      
      virtual BOOL OnDrag( const ivec3 &Delta, const ivec2 &LocalMousePos )
      {
        UpdateValue(LocalMousePos);
        return true;
      } /* End of 'OnDrag' function */

      virtual BOOL OnMouseDown( const ivec2 &LocalMousePos )
      {
        UpdateValue(LocalMousePos);
        return true;
      } /* End of 'OnMouseDown' function */

      virtual BOOL OnMouseUp( const ivec2 &LocalMousePos )
      {
        UpdateValue(LocalMousePos);
        return true;
      } /* End of 'OnMouseUp' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        ivec2 ThumbRelativePos = ivec2((INT)((Value - Min) / (Max - Min) * (BorderSize.W - BorderSize.H)), 0);
        
        // Track
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.Track.Border.DefColor), ToRGB(Style.Track.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        // Thumb
        Canvas->Render2d.PutBar(GlobalPos + ThumbRelativePos + ivec2(BoxProps.BorderW), isize2(BorderSize.H), ToRGB(Style.Thumb.Border.DefColor), ToRGB(Style.Thumb.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        
        std::ostringstream Str;

        Str.width(6);
        Str.precision(3);
        Str << "Value: " << Value;

        Canvas->Render2d.PutStr(Str.str(), GlobalPos, Size, {8, 0}, (DWORD)render_2d::vert_align::eCenter | (DWORD)render_2d::hor_align::eLeft, 0, ContentDrawMask);
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

      VOID SetValue( FLT NewValue )
      {
        Value = std::clamp(NewValue, Min, Max);
        Redraw();
      } /* End of 'SetValue' function */

    }; /* End of 'button' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_range_h_