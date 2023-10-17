/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : ui_widget_block.h
  * PURPOSE     : Skinning project.
  *               Main ui handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 24.11.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <def.h>

#ifndef __ui_widget_block_h_
#define __ui_widget_block_h_

#include "ui/mapper_system/tmp_ui_mp.h"

#include "ui_widget.h"

namespace tmp
{
  namespace ui
  {
    /* Widget blocks namespace */
    namespace widget_block
    {
      class widget_block_def abstract
      {
      public:

        ivec2 Pos;
        size Size;

        //std::vector<widget::widget_def *> Widgets; // Widgets of this widget block

        /* 'widget_block_def' construct function.
         * ARGUMENTS:
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        widget_block_def( const ivec2 &NewPos, const size &NewSize ) :
          Pos(NewPos), Size(NewSize)
        {
        } /* End of 'widget_block_def' function */

        virtual ~widget_block_def( VOID )
        {
        }


        ///* Def widget block destruct function.
        // * ARUGMENTS: None.
        // * RETURNS: None.
        // */
        //~widget_block_def( VOID )
        //{
        //  for (auto *Widget : Widgets)
        //    delete Widget;
        //
        //  Widgets.clear();
        //} /* End of 'widget_block_def' function */

        /* Resize widget block function.
         * ARGUMENTS:
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        virtual VOID Resize( const size &NewSize )
        {
          Size = NewSize;
        } /* End of 'Resize' function */

        /* Move widget block function.
         * ARGUMENTS:
         *   - new pos:
         *       const ivec2 &NewPos;
         * RETURNS: None.
         */
        virtual VOID Move( const ivec2 &NewPos )
        {
          Pos = NewPos;
        } /* End of 'Resize' function */

        /* Draw virtual abstract function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        virtual VOID Draw( const ivec2 &ParentScreenPos ) = 0;

        /* Response virtual function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        virtual VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState )
        {
        } /* End of 'Response' function */

      private:

        /* Is cursor hovered virtual function.
         * ARGUMENTS:
         *   - cursor pos:
         *       const ivec2 &CursorPos;
         * RETURNS: None.
         */
        virtual BOOL IsHovered( const ivec2 &CursorPos )
        {
          if (CursorPos.X >= Pos.X && CursorPos.Y >= Pos.Y &&
              CursorPos.X < Pos.X + Size.W && CursorPos.Y < Pos.Y + Size.H)
            return 1;
          return 0;
        } /* End of 'IsHovered' funciton */

      }; /* End of 'widget_block_def' class */


      /* Button widget block class */
      class button : public widget_block_def
      {
        gfx &Gfx;                    // Button gfx

        std::string Name;            // Button name
        BOOL Value;                  // Value (in future will be mapper)
        
        widget::text NameWidget;     // Button name text widget
      
        widget::button ButtonWidget; // Button widget

      public:

        /* 'button' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new button name:
         *       const std::string &NewName;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new button weight:
         *       const INT &NewW;
         *   - value reference:
         *       const BOOL &NewValue;
         * RETURNS: None.
         */
        button( gfx &NewGfx, const std::string &NewName, const ivec2 &NewPos, const INT &NewW, const BOOL &DefValue ) :
          Gfx(NewGfx), widget_block_def(NewPos, size(NewW, props::ValueBlockH)),
          Value(DefValue),
          Name(NewName),
          ButtonWidget(NewGfx,
            ivec2(2 * props::FrameOffset + props::ButtonBlockButtonNameSizeRat * (NewW - 3 * props::FrameOffset), props::FrameOffset),
            size((1 - props::ButtonBlockButtonNameSizeRat) * (NewW - 3 * props::FrameOffset), props::ValueBlockLineH),
            Value),
          NameWidget(NewGfx,
            NewName,
            {props::FrameOffset, props::FrameOffset},
            size(props::ButtonBlockButtonNameSizeRat * (NewW - 3 * props::FrameOffset), props::ValueBlockLineH))
        {
        } /* End of 'button' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          //Gfx.PutRoundBar(ParentScreenPos + Pos, Size, props::StdWidgetBlockR, color::bar::Frame, color::bar::Space);
          //Gfx.PutBar(ParentScreenPos + Pos, Size, color::bar::Frame, color::bar::Space, props::StdWidgetBlockR, gfx::RV_ALL);
          ButtonWidget.Draw(ParentScreenPos + Pos);
          NameWidget.Draw(ParentScreenPos + Pos);
        } /* End of 'Draw' function */

        /* Response override function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override
        {
          ButtonWidget.Response(CursorPos - Pos, MouseState);
          NameWidget.Response(CursorPos - Pos, MouseState);
        } /* End of 'Response' function */

      }; /* End of 'button' class */

      /* Number editor widget block class */
      class number_editor : public widget_block_def
      {
        gfx &Gfx;

        std::string Name;

        FLT Value, Min, Max;                   // Value (in future will be mapper)
        
        widget::text NameWidget;               // Number editor name text widget
        widget::number_editor NumEditorWidget; // Number editor widget

      public:

        /* 'number_editor' without bolders construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new button name:
         *       const std::string &NewName;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new w:
         *       const INT &NewW;
         *   - value, min, max reference:
         *       const FLT &DefValue, &NewMin, NewMax;
         * RETURNS: None.
         */
        number_editor( gfx &NewGfx, const std::string &NewName, const ivec2 &NewPos, const INT &NewW, const FLT &DefValue ) :
          Gfx(NewGfx), widget_block_def(NewPos, size(NewW, props::ValueBlockH)),
          Name(NewName),
          Value(DefValue), Min(DefValue), Max(DefValue),
          NumEditorWidget(NewGfx,
            ivec2(props::FrameOffset, props::FrameOffset),
            size(NewW - 2 * props::FrameOffset, props::ValueBlockLineH),
            Value,
            props::ValueNameStdSize.W + 2 * props::FrameOffset),
          NameWidget(NewGfx,
            NewName,
            {props::FrameOffset * 2, props::FrameOffset},
            props::ValueNameStdSize)
        {
        } /* End of 'button' function */

        /* 'number_editor' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new button name:
         *       const std::string &NewName;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const INT &NewW;
         *   - value, min, max reference:
         *       const FLT &DefValue, &NewMin, NewMax;
         * RETURNS: None.
         */
        number_editor( gfx &NewGfx, const std::string &NewName, const ivec2 &NewPos, const INT &NewW, const FLT &DefValue, const FLT &NewMin, const FLT &NewMax ) :
          Gfx(NewGfx), widget_block_def(NewPos, size(NewW, props::ValueBlockH)),
          Value(DefValue), Min(NewMin), Max(NewMax),
          Name(NewName),
          NumEditorWidget(NewGfx,
            ivec2(props::FrameOffset, props::FrameOffset),
            size(NewW - 2 * props::FrameOffset, props::ValueBlockLineH),
            Min, Max, Value,
            props::FrameOffset + props::ButtonBlockButtonNameSizeRat * (NewW - 3 * props::FrameOffset)),
          NameWidget(NewGfx,
            Name,
            {2 * props::FrameOffset, props::FrameOffset},
            size(props::ButtonBlockButtonNameSizeRat * (NewW - 3 * props::FrameOffset), props::ValueBlockLineH))
        {
        } /* End of 'button' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          //Gfx.PutRoundBar(ParentScreenPos + Pos, Size, props::StdWidgetBlockR, color::bar::Frame, color::bar::Space);
          //Gfx.PutBar(ParentScreenPos + Pos, Size, color::bar::Frame, color::bar::Space, props::StdWidgetBlockR, gfx::RV_ALL);
          NumEditorWidget.Draw(ParentScreenPos + Pos);
          NameWidget.Draw(ParentScreenPos + Pos);
        } /* End of 'Draw' function */

        /* Response override function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override
        {
          NumEditorWidget.Response(CursorPos - Pos, MouseState);
          NameWidget.Response(CursorPos - Pos, MouseState);

          //Mapper = Value;

        } /* End of 'Response' function */

      }; /* End of 'number_editor' class */


      /* Just header widget block class */
      class header: public widget_block_def
      {
        gfx &Gfx;

        widget::text NameWidget; // Number editor name text widget

      public:

        /* 'header' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new header name:
         *       const std::string &NewName;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const size &NewSize;
         *   - new gap:
         *       const INT &Gap;
         * RETURNS: None.
         */
        header( gfx &NewGfx, const std::string &NewName, const ivec2 &NewPos, const size &NewSize, const INT &Gap ) :
          Gfx(NewGfx), widget_block_def(NewPos, NewSize),
          NameWidget(NewGfx,
            NewName,
            {Gap, 0},
            NewSize - size(2 * Gap, 0))
        {
        } /* End of 'header' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          //Gfx.PutRoundBar(ParentScreenPos + Pos, Size, props::StdWidgetBlockR, color::bar::Frame, color::bar::Space);
          Gfx.PutBar(ParentScreenPos + Pos, Size, 0, color::HeaderBack, props::StdWidgetBlockR, gfx::RV_LEFT_TOP | gfx::RV_RIGHT_TOP);
          NameWidget.Draw(ParentScreenPos + Pos);
        } /* End of 'Draw' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         *   - other r:
         *       const INT NewR;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos, const INT NewR )
        {
          //Gfx.PutRoundBar(ParentScreenPos + Pos, Size, props::StdWidgetBlockR, color::bar::Frame, color::bar::Space);
          Gfx.PutBar(ParentScreenPos + Pos, Size, 0, color::HeaderBack, NewR, gfx::RV_LEFT_TOP | gfx::RV_RIGHT_TOP);
          NameWidget.Draw(ParentScreenPos + Pos);
        } /* End of 'Draw' function */

        /* Response override function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override
        {
          NameWidget.Response(CursorPos - Pos, MouseState);

        } /* End of 'Response' function */

      }; /* End of 'header' class */

      /* 2d viewport widget block class */
      class viewport_3d : public widget_block_def
      {
        gfx &Gfx;

        // ::tmp::window_context &ParentCtx;
        
        // ::tmp::viewport_3d ViewPort;

      public:

        /* 'viewport_3d' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const size &NewSize;
         *   - parent window context reference:
         *       window_context &NewParentCtx;
         *   - default camera:
         *       const camera &NewCamera;
         *   - new current scene:
         *       scene *NewScene;
         * RETURNS: None.
         */
        viewport_3d( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize/*,
            window_context &NewParentCtx, const camera &Ne  wCamera, scene *NewScene = nullptr */) :
          Gfx(NewGfx), widget_block_def(NewPos, NewSize)/*,
            ParentCtx(NewParentCtx),
            ViewPort(NewParentScreenPos + NewPos, NewSize, NewCamera, NewScene)*/
        {
          // ParentCtx.AddViewPort(&ViewPort);
        } /* End of 'viewport_3d' function */
        
        /* Switch scene function.
         * ARGUMENTS:
         *   - new scene pointer:
         *       scene *NewScene;
         * RETURNS: None.
         * /
        VOID SwitchScene( scene *NewScene )
        {
          ViewPort.SwitchScene(NewScene);
        } /* End of 'SwitchScene' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          // ViewPort.Render(); ???
          // Or we will draw viewport through context OnTimer ???
        } /* End of 'Draw' function */

        /* Response override function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override
        {
          // ViewPort.Response();
        } /* End of 'Response' function */
      }; /* End of 'viewport_3d' class */

    } /* end of 'widget_block' namespace */
  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

#endif // __ui_widget_block_h_

/* END OF 'ui_widget_block.h' FILE */