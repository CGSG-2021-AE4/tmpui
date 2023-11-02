#include <sstream>

#ifndef __ui_controls_range_h_
#define __ui_controls_range_h_

#include "../frame_render.h"
#include "../entity.h"

namespace ui
{
  namespace controls
  {
    /* Range style values struct */
    struct range_style
    {
      box_style
        Left,  // Left range part style
        Right; // Right range part style
    }; /* End of 'range_style' struct */

    /* Range props struct */
    struct range_props
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
      range_style Style {};
    }; /* End of 'range_props' struct */

    struct full_range_props : range_props
    {
      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex = { .Basis = flex_basis_type::eFixed };

      box_props BoxProps { .MarginW = 2, .BorderW = 2 };
    }; /* End of 'full_range_props' struct */

    /* Range class */
    class range : public entity
    {
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
      range_style Style {};

    public:
  
      /* Contsructor function */
      range( const range_props &NewProps ) :
        entity(full_range_props(NewProps)),
        Style(NewProps.Style),
        Min(NewProps.Min),
        Max(NewProps.Max),
        Value(NewProps.Value)
      {
      } /* End of 'slider' function */

      VOID UpdateValue( const ivec2 &LocalMousePos )
      {
        Value = Min + ((FLT)LocalMousePos.X - BoxProps.BorderW) / BorderSize.W * (Max - Min);
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
        FLT SeparationLineX = (Value - Min) / (Max - Min) * BorderSize.W + BoxProps.BorderW;
        // Left
        Canvas->Render2d.FillBar(GlobalPos, isize2(SeparationLineX, Size.H), ToRGB(Style.Left.Space.DefColor), SelfDrawMask);
        // Right
        Canvas->Render2d.FillBar(GlobalPos + ivec2(SeparationLineX, 0), isize2(Size.W - SeparationLineX, Size.H), ToRGB(Style.Right.Space.DefColor), SelfDrawMask);
        // Frame
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.Left.Border.DefColor), BoxProps.BorderW, SelfDrawMask);
        
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

    }; /* End of 'range' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_range_h_