/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : ui_panel.h
  * PURPOSE     : Skinning project.
  *               Main ui panel handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 13.12.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <def.h>
#include <cstdarg>

#ifndef __ui_panel_h_
#define __ui_panel_h_

#include "ui_widget_block.h"

namespace tmp
{
  namespace ui
  {
    namespace panel
    {
      class panel_def abstract
      {
      public:

        ivec2 Pos;
        size Size;

        std::vector<widget_block::widget_block_def *> WidgetBlocks; // Widget blocks of this functional piece

        /* 'panel_def' construct function.
          * ARGUMENTS:
          *   - new pos:
          *       const ivec2 &NewPos;
          *   - new size:
          *       const size &NewSize;
          * RETURNS: None.
          */
        panel_def( const ivec2 &NewPos, const size &NewSize ) :
          Pos(NewPos), Size(NewSize)
        {
        } /* End of 'panel_def' function */

        /* Def panel destruct function.
         * ARUGMENTS: None.
         * RETURNS: None.
         */
        ~panel_def( VOID )
        {
          for (auto *WidgetBlock : WidgetBlocks)
            delete WidgetBlock;

          WidgetBlocks.clear();
        } /* End of 'panel_def' function */

        /* Resize panel function.
         * ARGUMENTS:
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        virtual VOID Resize( const size &NewSize )
        {
          Size = NewSize;
        } /* End of 'Resize' function */

        /* Move panel function.
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

      }; /* End of 'panel_def' class */

      // Std context panel class
      class context : public panel_def
      {
        gfx &Gfx; // Button gfx

        std::string PanelName;

        widget_block::header Header;

        // !!! Debug temp
        //widget::toggle ToggleMenu;
        INT ToggleMenuCurIndex;

        public:

        /* 'context' construct function.
         * ARGUMENTS:
         *   - gfx refernece:
         *       const gfx &Gfx;
         *   - new panel name:
         *       const std::string &NewName;
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new context panel W:
         *       const FLT &NewContextW;
         * RETURNS: None.
         */
        context( gfx &NewGfx, const std::string &NewName, const ivec2 &NewPos, const FLT &NewContextW ) :
          Gfx(NewGfx), panel_def(NewPos, size(NewContextW, props::StdHeaderH + props::PanelRGap)),
          Header(NewGfx, NewName, ivec2(0, 0), size(NewContextW, props::StdHeaderH), props::PanelRGap),
          ToggleMenuCurIndex(0),
          PanelName(NewName)//,
          //ToggleMenu(Gfx, ivec2(100, 100), props::ToggleSize, {"First", "Second"}, ToggleMenuCurIndex)
        {
        } /* End of 'context' function */

        //template<typename Type>
        //  /* Add widget block function.
        //   * ARGUMENTS:
        //   *   - new widget name:
        //   *       const std::string &NewName;
        //   *   - other params for new widget block:
        //   *       ...;
        //   * RETURNS: None.
        //   */
        //  VOID AddWidgetBlock( const std::string &NewName, FLT A... )
        //  {
        //    WidgetBlocks.push_back(new Type(Gfx, NewName, ivec2(0, WidgetBlocks.size() * (props::StdWidgetBlockSize.H + props::WidgetBlockGap)), ParentScreenPos + Pos, A... ));
        //  } /* End of 'AddWidgetBlock' function */

        template<typename Type, typename ArgType>
          /* Add widget block with one bool function.
           * ARGUMENTS:
           *   - new widget name:
           *       const std::string &NewName;
           *   - one arggtype aditional argument:
           *       const ArgType &Arg1;
           * RETURNS: None.
           */
          VOID AddWidgetBlock( const std::string &NewName, const ArgType &Arg1 )
          {
            WidgetBlocks.push_back(new Type(Gfx, NewName, ivec2(props::PanelRGap, props::StdHeaderH + (INT)WidgetBlocks.size() * (props::ValueBlockH)), Size.W - 2 * props::PanelRGap, Arg1));

            Size.H += props::ValueBlockH;

            if ((INT)WidgetBlocks.size() != 1)
              Size.H += props::WidgetBlockGap;
          } /* End of 'AddWidgetBlock' function */

        template<typename Type>
          /* Add widget block with three flt function.
           * ARGUMENTS:
           *   - new widget name:
           *       const std::string &NewName;
           *   - three floats aditional arguments:
           *       const FLT &Arg1, &Arg2, &Arg3;
           * RETURNS: None.
           */
          VOID AddWidgetBlock( const std::string &NewName, const FLT &Arg1, const FLT &Arg2, const FLT &Arg3 )
          {
            WidgetBlocks.push_back(new Type(Gfx, NewName, ivec2(props::PanelRGap, props::StdHeaderH + (INT)WidgetBlocks.size() * (props::ValueBlockH)), Size.W - 2 * props::PanelRGap, Arg1, Arg2, Arg3));

            Size.H += props::ValueBlockH;

            if ((INT)WidgetBlocks.size() != 1)
              Size.H += props::WidgetBlockGap;
          } /* End of 'AddWidgetBlock' function */

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override
        {
          //Gfx.PutRoundBar(ParentScreenPos + Pos, Size, props::StdWidgetBlockR, color::bar::Frame, color::ContextMenuBack);
          Gfx.PutBar(ParentScreenPos + Pos, Size, 0, color::ContextMenuBack, props::PanelRGap, gfx::RV_ALL);
          Header.Draw(ParentScreenPos + Pos, props::PanelRGap);
          for (auto *WidgetBlock : WidgetBlocks)
            WidgetBlock->Draw(ParentScreenPos + Pos);
          Gfx.PutBar(ParentScreenPos + Pos, Size, color::White, 0, props::PanelRGap, gfx::RV_ALL);

          //ToggleMenu.Draw(ParentScreenPos + Pos);
          
          //
          //Gfx.PutText(PanelName, ParentScreenPos + Pos + ivec2(props::FrameOffset, 0), size(Size.W - 2 * props::FrameOffset, props::LineH), props::DefTextFormat);

          
          // !!!!!!!!!!!!! Debug for new round bar
          //Gfx.PutRoundBar(ivec2(300, 200), size(100, 100), 20, 0xFFFFFFFF, 0xFF404040);

          //Gfx.PutBar(ivec2(300, 200), size(100, 100), 0xFFFFFFFF, 0xFF404040, 20, 0);

          //Gfx.PutSeparatedBar(ivec2(300, 200), size(100, 100), 0xFFFFFFFF, 0xFF00FFFF, 0xFF404040, ivec2(10, 0), 20, gfx::RV_ALL);
          //Gfx.PutBar(ivec2(430, 200), size(100, 100), 0xFFFFFFFF, 0xFF404040, 20, gfx::RV_LEFT_TOP | gfx::RV_RIGHT_BOTTOM);
          //Gfx.PutBar(ivec2(300, 330), size(100, 100), 0xFFFFFFFF, 0xFF404040, 20, gfx::RV_RIGHT_TOP | gfx::RV_LEFT_BOTTOM);
          //Gfx.PutBar(ivec2(430, 330), size(100, 100), 0xFFFFFFFF, 0xFF404040, 20, gfx::RV_RIGHT_TOP | gfx::RV_LEFT_BOTTOM | gfx::RV_LEFT_TOP | gfx::RV_RIGHT_BOTTOM);
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
          ivec2 TempPos = CursorPos - Pos;

          for (auto *WidgetBlock : WidgetBlocks)
            WidgetBlock->Response(TempPos, MouseState);

        } /* End of 'Response' function */

      }; /* End of 'context' class */

      /* 3d viewport panel class */
      class viewport_3d : public panel_def
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
            window_context &NewParentCtx, const camera &NewCamera, scene *NewScene = nullptr */) :
          Gfx(NewGfx), panel_def(NewPos, NewSize)/*,
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
          Gfx.PutBar(ParentScreenPos + Pos, Size, 0xFF00FF00, 0xFF330033);
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

    } /* end of 'panel' namespace */
  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

#endif // __ui_panel_h_

/* END OF 'ui_panel.h' FILE */