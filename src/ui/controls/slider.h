#ifndef __ui_controls_slider_h_
#define __ui_controls_slider_h_

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
      // Entity part
      std::string Id {"Slider"};
      ivec2 Pos {};
      isize2 Size {60, 30};
      
      // Slider part
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
      slider_style Style {};
    }; /* End of 'slidre_props' struct */

    struct full_slider_props : slider_props
    {
      layout_props LayoutProps {};
      box_props BoxProps { .MarginW = 2, .BorderW = 2 };
    }; /* End of 'full_slider_props' struct */

    /* Slider class */
    class slider : public entity
    {
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
      INT ThumbW = 0;
      slider_style Style {};

    public:
  
      /* Contsructor function */
      slider( const slider_props &NewProps ) :
        entity(full_slider_props(NewProps)),
        Style(NewProps.Style),
        Min(NewProps.Min),
        Max(NewProps.Max),
        Value(NewProps.Value)
      {
        ThumbW = Size.H - BoxProps.BorderW * 2;
      } /* End of 'slider' function */

      VOID UpdateValue( const ivec2 &LocalMousePos )
      {
        FLT TrackL = BorderSize.W - ThumbW;
        Value = Min + ((FLT)LocalMousePos.X - ThumbW / 2) / TrackL * (Max - Min);
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
        ivec2 ThumbRelativePos = ivec2((INT)((Value - Min) / (Max - Min) * (BorderSize.W - ThumbW)), 0);
        
        // Track
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.Track.Border.DefColor), ToRGB(Style.Track.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        // Thumb
        Canvas->Render2d.PutBar(GlobalPos + ThumbRelativePos + ivec2(BoxProps.BorderW), isize2(ThumbW), ToRGB(Style.Thumb.Border.DefColor), ToRGB(Style.Thumb.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        
        std::ostringstream Str;

        Str.width(6);
        Str.precision(3);
        Str << "Value: " << Value;

        Canvas->Render2d.PutStr(Str.str(), GlobalPos, Size, {8, 0}, (DWORD)render_2d::vert_align::eCenter | (DWORD)render_2d::hor_align::eLeft, 0, SelfDrawMask);
        // if (State > entity_state::eDef)
        //   if (State > entity_state::eHovered)
        //   {
        //     // Track
        //     Canvas->Render2d.PutBar(GlobalPos + TrackRelativePos, {Size.W - TrackRelativePos.X * 2, Style.TrackW}, ToRGB(Style.Track.Border.DefColor), ToRGB(Style.Track.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        //     // Thumb
        //     Canvas->Render2d.PutBar(GlobalPos + ThumbRelativePos, Style.ThumbSize, ToRGB(Style.Thumb.Border.DefColor), ToRGB(Style.Thumb.Space.DefColor), BoxProps.BorderW, SelfDrawMask);
        //   }
        //   else
        //   {
        //   }
        // else
        //   ;// Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Value ? Style.PressedBColor : Style.DefBColor), ToRGB(Value ? Style.PressedColor : Style.DefColor), BoxProps.BorderW, SelfDrawMask);

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

#endif // __ui_controls_div_h_