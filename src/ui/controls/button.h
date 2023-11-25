#ifndef __ui_controls_button_h_
#define __ui_controls_button_h_

#include <functional>

#include "../entity.h"

namespace ui
{
  namespace controls
  {
    /* Button style values struct */
    struct button_style
    {
      vec3
        DefColor =      {0.35},       // Default state space color
        DefBColor =     {0.6},        // Default state border color
        PressedColor =  {0.5},        // Pressed state space color
        PressedBColor = {0.6},        // Pressed state border color
          
        DefHoverColor =    {0.35},    // Hover default state space color
        DefHoverBColor =   {0.8},     // Hover default state border color
        PressedHoverColor =    {0.5}, // Hover pressed state space color
        PressedHoverBColor =   {0.8}, // Hover pressed state border color
        
        ActiveColor =   {0.8},        // Active state space color
        ActiveBColor =  {0.8};        // Active state border color
    }; /* End of 'button_style' struct */

    class button;

    struct button_props
    {
      BOOL IsPress {0};
      std::string Str;

      std::function<VOID ( button *Button )> OnClickCallBack {[](button *Button ){}};
      std::function<VOID ( button *Button )> OnChangeCallBack {[](button *Button ){}};
    }; /* Ebd if 'button_props' struct */

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::button>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {10};
      size_type MinSize {size_ref::eMinContent};
      size_type MaxSize {size_ref::eNone};

      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      BOOL Value {0};

      controls::button_style Style {};
      controls::button_props Props {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Button class */
    class button : public entity
    {
      button_style Style;
      button_props Props;
      BOOL Value;
      
    public:
  
      /* Contsructor function */
      button( const entity_props<button> &NewProps ) :
        entity(NewProps),
        Style(NewProps.Style),
        Props(NewProps.Props),
        Value(NewProps.Value)
      {
      } /* End of 'button' function */

      isize2 GetMaxContent( VOID ) override
      {
        return {render_2d::FontW * (INT)Props.Str.size(), render_2d::FontH};
      } /* End of 'GetMaxContent' function */

      /* Value getter function */
      BOOL GetValue( VOID ) const
      {
        return Value;
      } /* End of 'GetValue' function */

      /* Value setter function */
      VOID SetValue( BOOL NewValue )
      {
        Value = NewValue;
        Redraw();
      } /* End of 'SetValue' function */

      /* Value setter unsafe function - used in radio button */
      VOID SetValueUnsafe( BOOL NewValue )
      {
        Value = NewValue;
      } /* End of 'SetValue' function */

      /* Button string setter function */
      BOOL SetStr( const std::string &NewStr )
      {
        Props.Str = NewStr;
        OnUpdateContent();
      } /* End of 'SetStr' function */

      BOOL OnClick( const ivec2 &LocalMousePos ) override
      {
        if (Props.IsPress)
        {
          Value = !Value;
          if (Value)
            Props.OnClickCallBack(this);
          Props.OnChangeCallBack(this);
        }
        else
          Props.OnClickCallBack(this);

        return true;
      } /* End of 'OnClick' function */

      virtual BOOL OnMouseDown( const ivec2 &LocalMousePos )
      {
        return true;
      } /* End of 'OnMouseDown' function */

      virtual BOOL OnMouseUp( const ivec2 &LocalMousePos )
      {
        return true;
      } /* End of 'OnMouseUp' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        if (State > entity_state::eDef)
        {
          if (State == entity_state::eHovered)
            Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Value ? Style.PressedHoverBColor : Style.DefHoverBColor), ToRGB(Value ? Style.PressedHoverColor : Style.DefHoverColor), BoxProps.BorderW, SelfDrawMask);
          else
            Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.ActiveBColor), ToRGB(Style.ActiveColor), BoxProps.BorderW, SelfDrawMask);
        }
        else
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Value ? Style.PressedBColor : Style.DefBColor), ToRGB(Value ? Style.PressedColor : Style.DefColor), BoxProps.BorderW, SelfDrawMask);

        Canvas->Render2d.PutStr(Props.Str, GlobalPos, Size, 0, (DWORD)render_2d::vert_align::eCenter | (DWORD)render_2d::hor_align::eCenter, 0, ContentDrawMask);
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

    }; /* End of 'button' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_div_h_