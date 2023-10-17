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

#include <def.h>

#ifndef __widget_h_
#define __widget_h_

#include <gfx/gfx_def.h>
#include "ui_support.h"

namespace tmp
{
  namespace ui
  {
    // Widgets namespace
    namespace widget
    {
      /* Def widget class */
      class widget_def abstract
      {
      public:

        ivec2 Pos;
        size Size;
        BOOL IsWidgetHovered, IsWidgetGrabed;
        
        /* 'widget_def' construct function.
         * ARGUMENTS:
         *   - new pos:
         *       const ivec2 &NewPos;
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        widget_def( const ivec2 &NewPos, const size &NewSize );

        /* 'widget_def' destruct function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        virtual ~widget_def( VOID );

        /* Resize widget function.
         * ARGUMENTS:
         *   - new size:
         *       const size &NewSize;
         * RETURNS: None.
         */
        virtual VOID Resize( const size &NewSize );

        /* Move widget function.
         * ARGUMENTS:
         *   - new pos:
         *       const ivec2 &NewPos;
         * RETURNS: None.
         */
        virtual VOID Move( const ivec2 &NewPos );

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
        virtual VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState );

      protected:

        /* Is cursor hovered virtual function.
         * ARGUMENTS:
         *   - cursor pos:
         *       const ivec2 &CursorPos;
         * RETURNS: None.
         */
        virtual BOOL IsHovered( const ivec2 &CursorPos );

        // Need mappers vars
        // std::map *Vars;
        // mAy be callback functions 
      }; /* End of 'widget_def' class */

      /* Button widget class */
      class button : public widget_def
      {
        gfx &Gfx;
        BOOL &Value;

      public:

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
        button( gfx &NewGfx, const ivec2 &NewPos, BOOL &NewValue );

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
        button( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, BOOL &NewValue );

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override;

        /* Response virtual function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override;

      }; /* End of 'button' class */

      /* Text class */
      class text : public widget_def
      {
        gfx &Gfx;
      
        std::string Str {};

      public:

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
        text( gfx &NewGfx, const std::string &NewStr, const ivec2 &NewPos, const size &NewSize = props::TextSize );

        /* 'text' destruct function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        ~text( VOID );

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override;

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         *   - new string:
         *       const std::string &NewStr;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos, const std::string &NewStr );

      }; /* End of 'text' class */

      /* Number editor widget class */
      class number_editor : public widget_def
      {
        gfx &Gfx;

        FLT &Min, &Max, &Value;

        ivec2 TempOldMousePos {}; // In future will be returned

        INT ValueTextDelta {};
        BOOL BorderFlag {};

      public:

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
        number_editor( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, FLT &NewValue, const INT &NewValueTextDelta = 0 );

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
        number_editor( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, FLT &NewMin, FLT &NewMax, FLT &NewValue, const INT &NewValueTextDelta = 0 );

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override;

        /* Response override function.
         * ARGUMNETS:
         *   - cursor coords:
         *       const ivec2 &CursorPos;
         *   - cursor left button state:
         *       const CLICK_STATE &MouseState;
         * RETURNS: None.
         */
        VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override;

      }; /* End of 'button' class */

      /* Toggle widget class */
      class toggle : public widget_def
      {
        gfx &Gfx;

        std::vector<std::string> PNames;

        INT &CurPValue;

      public:

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
        toggle( gfx &NewGfx, const ivec2 &NewPos, const size &NewSize, const std::vector<std::string> &NewPNames, INT &NewCurPValue );

        /* Draw function.
         * ARUGMENTS:
         *   - parent screen pos:
         *       const ivec2 &ParentScreenPos;
         * RETURNS: None.
         */
        VOID Draw( const ivec2 &ParentScreenPos ) override;

        ///* Response override function.
        // * ARGUMNETS:
        // *   - cursor coords:
        // *       const ivec2 &CursorPos;
        // *   - cursor left button state:
        // *       const CLICK_STATE &MouseState;
        // * RETURNS: None.
        // */
        //VOID Response( const ivec2 &CursorPos, const CLICK_STATE &MouseState ) override;

      }; /* End of 'toggle' class */

      // text
      // scrollbar
      // tooltip
      // icone
    
      // radio button
      // checker button
      // slider
      // number editor - my be combine with slider
    
      // toggle button
      // text editor
      // color selecter
    
      // rec toggle lists
      // toolbar
      // graph

    } /* end of 'widget' namespace */
  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

#endif // __widget_h_

/* END OF 'widget.h' FILE*/