/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : ui_support.cpp
  * PURPOSE     : Skinning project.
  *               Main ui support module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 13.12.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "ui_support.h"

#define UI_SCALE(A) (A * props::StdScale)
#define GOLDEN_RATIO 1.6180339887

namespace tmp
{
  namespace ui
  {
    // Color setup
    DWORD
      color::std_clickable_widget_color::Active = 0xFF7F7F7F,
      color::std_clickable_widget_color::Disable = 0xFF313131,
      color::std_clickable_widget_color::ActiveHovered = 0xFF707070,
      color::std_clickable_widget_color::DisableHovered = 0xFF1F1F1F,
      color::std_clickable_widget_color::Frame = 0xFFC6C6C6,

      color::bar::Space = 0xFF616161,
      color::bar::Frame = 0xFFA1A1A1,

      color::White = 0xFFFFFFFF,
      color::Black = 0xFF000000,
      color::Text = RGB(255 * 0.30, 255 * 0.47, 255 * 0.8),
      color::TopMenuBack = 0xFF585858,
      color::ContextMenuBack = 0xFF3E3E3E,
      color::ToggleMenuBack = 0xFF191919,
      color::HeaderBack = 0xFF242424,
      color::ToolTip = 0xFF585858;

    // Props setup
   
    FLT
      props::StdScale = 1,
      props::MouseSens = 0.001,
      props::ButtonBlockButtonNameSizeRat = 1 / GOLDEN_RATIO;

    INT
      props::FrameOffset = UI_SCALE(4),
      props::LineH = UI_SCALE(20),
      props::WidgetBlockGap = UI_SCALE(0),
      props::StdWidgetBlockR = UI_SCALE(5),
      props::PanelRGap = UI_SCALE(6),
      props::StdHeaderH = props::LineH + props::FrameOffset,
      props::StdViewportPanelsGap = UI_SCALE(20),
      props::ViewportContextW = UI_SCALE(180),
      props::ValueBlockLineH = props::LineH,
      props::ValueBlockH = props::ValueBlockLineH + 2 * props::FrameOffset;

    size
      props::StdWidgetBlockSize = {(INT)UI_SCALE(160), 2 * props::FrameOffset + props::LineH},
      props::ButtonSize = {(INT)(StdWidgetBlockSize.W * (1 - 1 / GOLDEN_RATIO) - 2 * FrameOffset), LineH},
      props::ValueNameStdSize = {(INT)(StdWidgetBlockSize.W * (1 / GOLDEN_RATIO) - 2 * FrameOffset), LineH},
      props::ToggleSize = {StdWidgetBlockSize.W, LineH},
      props::ButtonBlockSize = StdWidgetBlockSize,
      props::SliderSize = {100, 20},
      props::TextSize = {(INT)UI_SCALE(100), LineH},
      props::ContextPanelSize = {props::StdWidgetBlockSize.W, props::StdHeaderH};

    BYTE
      props::DefTextFormat = font::TAH_LEFT | font::TAV_CENTER | font::TAP_CLIP | font::TAP_SINGLELINE;

  } /* end of 'ui' namespace */
} /* end of 'tmp' namespace */

/* END OF 'ui_support.cpp' FILE */