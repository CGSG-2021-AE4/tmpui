/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : widget.h
  * PURPOSE     : Skinning project.
  *               Main widget handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 24.11.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <sstream>

#include "ui_widget.h"

namespace tmp
{
  namespace ui
  {
    namespace widget
    {
      /* 'widget_def' construct function.
       * ARGUMENTS:
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - new size:
       *       const size &NewSize;
       * RETURNS: None.
       */
      widget_def::widget_def( const ivec2 &NewPos, const size &NewSize ) :
        Pos(NewPos), Size(NewSize), IsWidgetHovered(0)
      {
      } /* End of 'widget_def::widget_def' construct function */

      /* 'widget_def' destruct function.
       * ARGUMENTS: None
       * RETURNS: None.
       */
      widget_def::~widget_def( VOID )
      {
      } /* End of 'widget_def::~widget_def' function */

      /* Resize widget function.
       * ARGUMENTS:
       *   - new size:
       *       const size &NewSize;
       * RETURNS: None.
       */
      VOID widget_def::Resize( const size &NewSize )
      {
        Size = NewSize;
      } /* End of 'Resize' function */

      /* Move widget function.
       * ARGUMENTS:
       *   - new pos:
       *       const ivec2 &NewPos;
       * RETURNS: None.
       */
      VOID widget_def::Move( const ivec2 &NewPos )
      {
        Pos = NewPos;
      } /* End of 'Resize' function */

      /* Response virtual function.
       * ARGUMNETS:
       *   - cursor coords:
       *       const ivec2 &CursorPos;
       *   - cursor left button state:
       *       const CLICK_STATE &MouseState;
       * RETURNS: None.
       */
      VOID widget_def::Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState )
      {
        IsWidgetHovered = IsHovered(CursorPos);
        IsWidgetGrabed = IsWidgetHovered & (MouseState != CS_RELEASED);
      } /* End of 'widget_def::Response' function */

      /* Is cursor hovered virtual function.
       * ARGUMENTS:
       *   - cursor pos:
       *       const ivec2 &CursorPos;
       * RETURNS: None.
       */
      BOOL widget_def::IsHovered( const ivec2 &CursorPos )
      {
        if (CursorPos.X >= Pos.X && CursorPos.Y >= Pos.Y &&
            CursorPos.X < Pos.X + Size.W && CursorPos.Y < Pos.Y + Size.H)
          return 1;
        return 0;
      } /* End of 'widget_def::IsHovered' funciton */

      /*
       * Button widget functions
       */

      /* 'button' construct function.
       * ARGUMENTS:
       *   - gfx refernece:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - value reference:
       *       BOOL &NewValue;
       * RETURNS: None.
       */
      button::button( gfx &NewGfx, const ivec2 &NewPos, BOOL &NewValue ) :
        Gfx(NewGfx), widget_def(NewPos, props::ButtonSize), Value(NewValue)
      {
      } /* End of 'button::button' function */

      /* 'button' construct function.
       * ARGUMENTS:
       *   - gfx refernece:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - new button size:
       *       const size &NewSize;
       *   - value reference:
       *       BOOL &NewValue;
       * RETURNS: None.
       */
      button::button( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, BOOL &NewValue ) :
        Gfx(NewGfx), widget_def(NewPos, NewSize), Value(NewValue)
      {
      } /* End of 'button::button' function */

      /* Draw function.
       * ARUGMENTS:
       *   - parent screen pos:
       *       const ivec2 &ParentScreenPos;
       * RETURNS: None.
       */
      VOID button::Draw( const ivec2 &ParentScreenPos )
      {
        // Draw frame and filled bar

        if (Value)
          if (IsWidgetHovered)
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::ActiveHovered, Size.H / 4.0, gfx::RV_ALL);
          else
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::Active, Size.H / 4.0, gfx::RV_ALL);
        else
          if (IsWidgetHovered)
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::DisableHovered, Size.H / 4.0, gfx::RV_ALL);
          else
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::Disable, Size.H / 4.0, gfx::RV_ALL);
        //Gfx.PutText("My first button text", Pos + props::FrameOffset, Size - props::ButtonTextOffsetSize);
      } /* End of 'button::Draw' function */

      /* Response virtual function.
       * ARGUMNETS:
       *   - cursor coords:
       *       const ivec2 &CursorPos;
       *   - cursor left button state:
       *       const CLICK_STATE &MouseState;
       * RETURNS: None.
       */
      VOID button::Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState )
      {
        IsWidgetHovered = IsHovered(CursorPos);
        IsWidgetGrabed = IsWidgetHovered & (MouseState != CS_RELEASED);

        if (IsWidgetHovered && MouseState == CS_CLICKED)
          Value = 1 - Value;
      } /* End of 'Response' function */

      /*
       * Text widget functions
       */

      /* 'text' construct function.
       * ARGUMENTS:
       *   - gfx refernece:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - input string:
       *       const std::string &NewStr;
       * RETURNS: None.
       */
      text::text( gfx &NewGfx, const std::string &NewStr, const ivec2 &NewPos, const size &NewSize ) :
        Gfx(NewGfx), widget_def(NewPos, NewSize), Str(NewStr)
      {
      } /* End of 'text::text' function */

      /* 'text' destruct function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      text::~text( VOID )
      {
        //Str.clear();
        //delete &Str;
      } /* End of 'text::~text' function */

      /* Draw function.
       * ARUGMENTS:
       *   - parent screen pos:
       *       const ivec2 &ParentScreenPos;
       * RETURNS: None.
       */
      VOID text::Draw( const ivec2 &ParentScreenPos )
      {
        // Put text
        Gfx.PutText(Str, ParentScreenPos + Pos, Size);
      } /* End of 'text::Draw' function */

      /* Draw function.
       * ARUGMENTS:
       *   - parent screen pos:
       *       const ivec2 &ParentScreenPos;
       *   - new string:
       *       const std::string &NewStr;
       * RETURNS: None.
       */
      VOID text::Draw( const ivec2 &ParentScreenPos, const std::string &NewStr )
      {
        Str = NewStr;
        
        // Put text
        Gfx.PutText(Str, ParentScreenPos + Pos, Size);
      } /* End of 'Draw' function */

      /*
       * Number editor widget functions
       */

      /* 'number_editor' without bolders construct function.
       * ARGUMENTS:
       *   - gfx refernece:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - new num editor size:
       *       const size &NewSize;
       *   - new def values:
       *       const FLT &NewValue;
       *   - value text delta:
       *       const INT &NewValueTextDelta;
       * RETURNS: None.
       */
      number_editor::number_editor( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, FLT &NewValue, const INT &NewValueTextDelta ) :
        Gfx(NewGfx), widget_def(NewPos, NewSize),
        Value(NewValue), Min(Value), Max(Value),
        ValueTextDelta(NewValueTextDelta),
        BorderFlag(0)
      {
      } /* End of 'button::button' function */

      /* 'number_editor' construct function.
       * ARGUMENTS:
       *   - gfx refernece:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - new num editor size:
       *       const size &NewSize;
       *   - new min, max, def values:
       *       const FLT &Min, &Max, &NewValue;
       *   - value text delta:
       *       const INT &NewValueTextDelta;
       * RETURNS: None.
       */
      number_editor::number_editor( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, FLT &NewMin, FLT &NewMax, FLT &NewValue, const INT &NewValueTextDelta ) :
        Gfx(NewGfx), widget_def(NewPos, NewSize),
        Value(NewValue), Min(NewMin), Max(NewMax),
        ValueTextDelta(NewValueTextDelta),
        BorderFlag(1)
      {
        TempOldMousePos = ivec2(0);
      } /* End of 'button::button' function */

      /* Draw function.
       * ARUGMENTS:
       *   - parent screen pos:
       *       const ivec2 &ParentScreenPos;
       * RETURNS: None.
       */
      VOID number_editor::Draw( const ivec2 &ParentScreenPos )
      {
        if (BorderFlag)
        {
          // Draw frame and filled bar
          INT D = Size.W * (Value - Min) / (Max - Min);

          if (!IsWidgetGrabed && IsWidgetHovered)
            Gfx.PutSeparatedBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::ActiveHovered, color::button::DisableHovered, ivec2(D, 0), Size.H / 4, gfx::RV_ALL);
          else
            Gfx.PutSeparatedBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::Active, color::button::Disable, ivec2(D, 0), Size.H / 4, gfx::RV_ALL);
          //if (!IsWidgetGrabed && IsWidgetHovered)
          //{
          //  Gfx.PutBar(ParentScreenPos + Pos, size(D, Size.H), color::button::Frame, color::button::ActiveHovered);
          //  Gfx.PutBar(ParentScreenPos + Pos + ivec2(D, 0), Size - size(D, 0), color::button::Frame, color::button::DisableHovered);
          //}
          //else
          //{
          //  Gfx.PutBar(ParentScreenPos + Pos, size(D, Size.H), color::button::Frame, color::button::Active);
          //  Gfx.PutBar(ParentScreenPos + Pos + ivec2(D, 0), Size - size(D, 0), color::button::Frame, color::button::Disable);
          //}
        }
        else
        {
          if (IsWidgetGrabed)
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::ActiveHovered, Size.H / 4, gfx::RV_ALL);
          else if (IsWidgetHovered)
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::DisableHovered, Size.H / 4, gfx::RV_ALL);
          else
            Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::Disable, Size.H / 4, gfx::RV_ALL);
        }
        
          
        std::ostringstream TempStr;

        // kall:

        TempStr.width(6);
        TempStr.precision(3);
        TempStr << Value;

        Gfx.PutText(TempStr.str(), ParentScreenPos + Pos + ivec2(ValueTextDelta, 0), Size - size(ValueTextDelta, 0), 0xFFFFFFFF, props::DefTextFormat);
        //Gfx.PutText(std::to_string(Value), ParentScreenPos + Pos, Size, 0xFFFFFFFF, font::TAV_CENTER | font::TAP_CLIP | font::TAP_SINGLELINE/* | gfx::TAH_CENTER*/);
      } /* End of 'button::Draw' function */

      /* Response override function.
       * ARGUMNETS:
       *   - cursor coords:
       *       const ivec2 &CursorPos;
       *   - cursor left button state:
       *       const CLICK_STATE &MouseState;
       * RETURNS: None.
       */
      VOID number_editor::Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState )
      {
        
        IsWidgetHovered = IsHovered(CursorPos);
        
        if (IsWidgetGrabed)
          IsWidgetGrabed = MouseState == CS_PRESSED;
        else
          IsWidgetGrabed = IsWidgetHovered & (MouseState == CS_CLICKED);

        if (IsWidgetGrabed)
          if (BorderFlag)
          {
            Value = ((CursorPos.X - Pos.X) / (FLT)Size.W) * (Max - Min) + Min;
            
            // Clipping
            if (Value > Max)
              Value = Max;
            else if (Value < Min)
              Value = Min;
          }
          else
            Value += (CursorPos.X - TempOldMousePos.X) * props::MouseSens;

        
        TempOldMousePos = CursorPos;

        //Value = sin(NumbereditorTempCounter / 10.0) / 2 + 0.5;
        //if (IsWidgetHovered && MouseState == CS_CLICKED)
        //  Value = 1 - Value;
      } /* End of 'number_editor' function */


      /*
       *** Toggle widget 
       */

      /* 'toggle' menu construct function.
       * ARGUMENTS:
       *   - gfx reference:
       *       const gfx &Gfx;
       *   - new pos:
       *       const ivec2 &NewPos;
       *   - new num editor size:
       *       const size &NewSize;
       *   - menu paragraphs names:
       *       const std::vector<std::string> &NewPNames;
       *   - mapper current menu paragraph index value;
       *       INT &NewCurPValue;
       * RETURNS: None.
       */
      toggle::toggle( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, const std::vector<std::string> &NewPNames, INT &NewCurPValue ) :
        Gfx(NewGfx), widget_def(NewPos, NewSize),
        PNames(NewPNames), CurPValue(NewCurPValue)
      {
        if (PNames.size() == 0)
          PNames.push_back("ERROR!!!");
      } /* End of 'toggle' function */

      /* Draw function.
       * ARUGMENTS:
       *   - parent screen pos:
       *       const ivec2 &ParentScreenPos;
       * RETURNS: None.
       */
      VOID toggle::Draw( const ivec2 &ParentScreenPos )
      {
        Gfx.PutBar(ParentScreenPos + Pos, Size, color::button::Frame, color::button::DisableHovered, Size.H / 4, gfx::RV_ALL);
        Gfx.PutTextIn(PNames[0], ParentScreenPos + Pos, Size, props::FrameOffset);
      } /* End of 'toggle::Draw' function */

    } /* end of 'widget' namespace */
  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

/* END OF 'widget.h' FILE */