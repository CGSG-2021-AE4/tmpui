/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : ui_editor.h
  * PURPOSE     : Skinning project.
  *               Main ui editor handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 13.12.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <def.h>

#ifndef __ui_editor_h_
#define __ui_editor_h_


#include "ui_panel.h"

namespace tmp
{
  namespace ui
  {
    namespace editor
    {
      class editor_def abstract
      {
      public:

        ivec2 Pos;
        size Size;

        std::vector<panel::panel_def *> Panels; // Panels of this editor

        /* 'editor_def' construct function.
         * ARGUMENTS:
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        editor_def( const ivec2 &NewPos, const size &NewSize ) :
          Pos(NewPos), Size(NewSize)
        {
        } /* End of 'editor_def' function */

        /* Def editor destruct function.
         * ARUGMENTS: None.
         * RETURNS: None.
         */
        virtual ~editor_def( VOID )
        {
          for (auto *Panel: Panels)
            delete Panel;
        
          Panels.clear();
        } /* End of 'panel_def' function */

        /* Resize editor function.
         * ARGUMENTS:
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        virtual VOID Resize( const size &NewSize )
        {
          Size = NewSize;
        } /* End of 'Resize' function */

        /* Move editor function.
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

      }; /* End of 'editor_def' class */

      /* Viewport 3d editor class */
      class viewport_3d : public editor_def
      {
        gfx &Gfx; // Draw context

        std::string EditorName;

        panel::viewport_3d Viewport;

        panel::context Context;

      public:

        /* 'viewport_3d' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new viewport editor name:
         *       const std::string &NewName;
         *   - draw window context reference:
         *       const context_window &WindowCtx;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new editor size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        viewport_3d( gfx &NewGfx, const std::string &NewName, /* const context_window &WindowCtx, */ const ivec2 &NewPos, const size &NewSize ) :
          Gfx(NewGfx), editor_def(NewPos, NewSize),
          EditorName(NewName),
          Viewport(NewGfx, NewPos, NewSize),
          Context(NewGfx, NewName + ":Context", ivec2(NewSize.W - props::ViewportContextW - props::StdViewportPanelsGap, props::StdViewportPanelsGap), props::ViewportContextW)
        {
          //Context.AddWidgetBlock<ui::widget_block::button, BOOL>("Value1:", (BOOL)0);
          //Context.AddWidgetBlock<ui::widget_block::number_editor, FLT>("Value2:", 3);
          //Context.AddWidgetBlock<ui::widget_block::number_editor>("Value3:", 3, 1, 4);

          for (INT i = 0; i < 30; i++)
            switch (rand() % 3)
            {
            case 0:
              Context.AddWidgetBlock<ui::widget_block::button, BOOL>("Value " + std::to_string(i) + ":", rand() % 2);
              break;
            case 1:
              Context.AddWidgetBlock<ui::widget_block::number_editor, FLT>("Value " + std::to_string(i) + ":", rand() % 20 + 10);
              break;
            case 2:
              Context.AddWidgetBlock<ui::widget_block::number_editor>("Value " + std::to_string(i) + ":", rand() % 10 + 15,  rand() % 10, rand() % 10 + 30);
              break;
            }
            
        } /* End of 'viewport_3d' function */

        /* Resize editor override function.
         * ARGUMENTS:
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        VOID Resize( const size &NewSize ) override
        {
          Size = NewSize;

          Viewport.Resize(Size);
          Context.Move(ivec2(NewSize.W - props::ViewportContextW - props::StdViewportPanelsGap, props::StdViewportPanelsGap));
        } /* End of 'Resize' function */

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
          Context.Response(CursorPos - Pos, MouseState);
        } /* End of 'Response' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          Viewport.Draw(ParentScreenPos + Pos);
          Context.Draw(ParentScreenPos + Pos);
        } /* End of 'Draw' function */

      }; /* End of 'viewport_3d' class */

    } /* end of 'editor' namespace */
  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

#endif // __ui_editor_h_

/* END OF 'ui_editor.h' FILE */