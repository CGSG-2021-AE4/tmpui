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
      // Slider part
      FLT
        Min = 0,
        Max = 1,
        Value = 0.5;
    }; /* End of 'range_props' struct */

    class range;

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::range>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {10};
      size_type MinSize {size_ref::eMinContent};
      isize2 MaxSize {10000};

      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      controls::range_style Style {};
      controls::range_props Props {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Range class */
    class range : public entity
    {
      range_props Props {};
      range_style Style {};

    public:
  
      /* Contsructor function */
      range( const entity_props<range> &NewProps ) :
        entity(NewProps),
        Style(NewProps.Style),
        Props(NewProps.Props)
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
        Props.Value = Props.Min + ((FLT)LocalMousePos.X - BoxProps.BorderW) / BorderSize.W * (Props.Max - Props.Min);
        Props.Value = std::clamp(Props.Value, Props.Min, Props.Max);
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
        FLT SeparationLineX = (Props.Value - Props.Min) / (Props.Max - Props.Min) * BorderSize.W + BoxProps.BorderW;
        // Left
        Canvas->Render2d.FillBar(GlobalPos, isize2(SeparationLineX, Size.H), ToRGB(Style.Left.Space.DefColor), SelfDrawMask);
        // Right
        Canvas->Render2d.FillBar(GlobalPos + ivec2(SeparationLineX, 0), isize2(Size.W - SeparationLineX, Size.H), ToRGB(Style.Right.Space.DefColor), SelfDrawMask);
        // Frame
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.Left.Border.DefColor), BoxProps.BorderW, SelfDrawMask);
        
        std::ostringstream Str;

        Str.width(6);
        Str.precision(3);
        Str << "Value: " << Props.Value;

        Canvas->Render2d.PutStr(Str.str(), GlobalPos, Size, {8, 0}, (DWORD)render_2d::vert_align::eCenter | (DWORD)render_2d::hor_align::eLeft, 0, SelfDrawMask);
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
        Props.Value = std::clamp(NewValue, Props.Min, Props.Max);
        Redraw();
      } /* End of 'SetValue' function */

    }; /* End of 'range' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_range_h_