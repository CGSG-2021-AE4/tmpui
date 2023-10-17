/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : rt.h
 * PURPOSE     : Raytracing project.
 *               Ray tracing main handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 24.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __rt_h_
#define __rt_h_

//#include "csg.h"
#include "..\..\frame\frame.h"
#include "..\..\mth\mth.h"
#include <vector>
#include "..\..\def.h"

#define BACK_GRD_COLOR RGB(204, 120, 77)
#define BACK_GRD_COLOR_VEC vec3(0.30, 0.47, 0.8)
#define RT_THESHOLD 0.001

#include "./gfx/gfx_def.h"
#include "./ui2/canvas.h"
#include "./ui2/controls.h"
#include "./ui/ui_editor.h"

namespace tmp
{
  // scene class
  class scene
  {
  public:

    gfx Gfx;

    ::ui::canvas *Canvas;

    BOOL ButtonValue {1};

    /* Default scene construct function */
    scene( const HWND &hWnd, const INT &NewW, const INT &NewH ) :
      Gfx(hWnd, size(NewW, NewH), ui::props::DefTextFormat),
      Canvas(new ::ui::canvas({0, 0}, {100, 100}, nullptr))
    {
      Gfx.SetFrame(0xF00FF00F);
    } /* End of 'scene' function */

    /* Scene deconstruct function */
    ~scene()
    {
      delete Canvas;
    } /* End of 'scene' function */

    /* Resize function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Resize( INT NewW, INT NewH )
    {
    } /* End of 'Resize' function */

    /* Ray tracing main render function
     * ARGUMENTS: None.
     * RETURNS: None
     */
    void Render( mth::cam<DBL> &Camera, frame &Fr )
    {
      //if (Fr.W != Gfx.FrameSize.W || Fr.H != Gfx.FrameSize.H)
      //  Fr.Resize(Gfx.FrameSize.W, Gfx.FrameSize.H);


      POINT CursorPos;
      static ui::CLICK_STATE MouseState;
      
      if (GetKeyState(VK_LBUTTON) & 0x8000)
      {
        if (MouseState == ui::CS_CLICKED)
          MouseState = ui::CS_PRESSED;
        else if (MouseState != ui::CS_PRESSED)
          MouseState = ui::CS_CLICKED;
      }
      else
        MouseState = ui::CS_RELEASED;
          

      GetCursorPos(&CursorPos);

      ScreenToClient(Gfx.hWnd, &CursorPos);

      static INT OldFrameTime;
      static DBL FPS = 0;

      OldFrameTime = std::clock();

      /*
       ************* DRAW INTERFACE BEGINNING *************
       */

      // Gfx.SetFrame(0xFF000000);
      ivec2 StartPos = ivec2(0);

      Gfx.PutText("FPS: " + std::to_string(FPS), ivec2(0, 0), size(100, 20));


      
      /*
       ************* DRAW INTERFACE END *************
       */

      INT NewFrameDelta = std::clock();

      FPS = CLOCKS_PER_SEC / (DBL)(NewFrameDelta - OldFrameTime);

      /*
      for (INT y = 0; y < Fr.H; y++)
        for (INT x = 0; x < Fr.W; x++)
        {
          Fr.PutPixel(x, y, RGB(x % 255, y % 255, (x * y) % 255));
        }
      */

      if (Gfx.FrameSize.W != Fr.W || Gfx.FrameSize.H != Fr.H)
      {
        INT TempW = std::min(Gfx.FrameSize.W, Fr.W), TempH = std::min(Gfx.FrameSize.H, Fr.H);
      
        for (INT y = 0; y < TempH; y++)
          // Fr.PutPixel(x, y, Gfx.Frame[y * Gfx.FrameSize.W + x]); //Gfx.
          memcpy(&Fr.Image[Fr.W * y], &Gfx.Frame[y * Gfx.FrameSize.W], TempW * sizeof(DWORD));
      }
      else
      {
        memcpy(Fr.Image, Gfx.Frame, Fr.W * Fr.H * sizeof(DWORD));
      }
    } /* End of 'Render function'*/
  };
}

#endif //__rt_h_
