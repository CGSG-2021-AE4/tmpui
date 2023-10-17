/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : ui_support.h
  * PURPOSE     : Skinning project.
  *               Main ui support handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 24.11.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <def.h>

#ifndef __ui_support_h_
#define __ui_support_h_

namespace tmp
{
  namespace ui
  {
    typedef enum
    {
      CS_CLICKED,
      CS_PRESSED,
      CS_RELEASED,
    } CLICK_STATE;

    class color
    {
    public:

      class std_clickable_widget_color
      {
      public:

        static DWORD
          Active,
          Disable,
          ActiveHovered,
          DisableHovered,
          Frame;
      }; /* End of 'std_clickable_widget_color' class */

      class slider : public std_clickable_widget_color
      {
      }; /* End of 'slider' class */

      class button : public std_clickable_widget_color
      {
      }; /* End of 'button' class */

      class number_editor : public std_clickable_widget_color
      {
      }; /* End of 'number_editor' class */

      class bar
      {
      public:

        static DWORD
          Space,
          Frame;
      }; /* End of 'bar' class */

      // Vars

      static DWORD
        White,
        Black,
        Text,
        TopMenuBack,
        ContextMenuBack,
        ToggleMenuBack,
        HeaderBack,
        ToolTip;
    }; /* End of 'draw_color' class */
  } /* end of 'ui' namespace */

  namespace ui
  {
    /* UI properies class */
    class props
    {
    public:

      static FLT
        StdScale,
        MouseSens,
        ButtonBlockButtonNameSizeRat;

      static INT
        FrameOffset,
        LineH,
        WidgetBlockGap,
        StdWidgetBlockR,
        PanelRGap,
        StdHeaderH,
        StdViewportPanelsGap,
        ViewportContextW,
        ValueBlockLineH,
        ValueBlockH;

      static size
        StdWidgetBlockSize,
        ButtonSize,
        ToggleSize,
        ButtonBlockSize,
        SliderSize,
        TextSize,
        ValueNameStdSize,
        ContextPanelSize;

      static BYTE
        DefTextFormat;

    }; /* End of 'props' class */

  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

#endif // __ui_support_h_

/* END OF 'ui_support.h' FILE */