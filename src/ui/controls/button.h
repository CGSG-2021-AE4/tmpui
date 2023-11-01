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

    /* Button props struct */
    struct button_props
    {
      BOOL IsPress = 0;
      std::function<VOID( VOID )> OnClickCallBack {[](){}};
      std::function<VOID( BOOL NewValue )> OnChangeCallBack {[]( BOOL NewValue ){}};
    }; /* End of 'div_props' struct */

    /* Button class */
    class button : public entity
    {
      button_style Style;
      button_props Props;
      BOOL Value;
      
    public:
  
      /* Contsructor function */
      button( const entity_props<button_props, button_style> &NewProps ) :
        entity(NewProps),
        Style(NewProps.Style),
        Props(NewProps.Props)
      {
      } /* End of 'button' function */
      
      BOOL OnClick( const ivec2 &LocalMousePos ) override
      {
        if (Props.IsPress)
        {
          Value = !Value;
          Props.OnChangeCallBack(Value);
        }
        else
          Props.OnClickCallBack();

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