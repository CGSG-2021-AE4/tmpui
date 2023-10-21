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
#include "./ui2/frame_render.h"
#include "./ui2/canvas.h"
#include "./ui2/controls.h"
#include "./ui/ui_editor.h"

/* Test class */
class test
{
public:

  ::ui::render_2d &Render2d;
  ::ui::canvas Canvas;

  /* Contructor function */
  test( ::ui::render_2d &NewRender2d ) :
    Render2d(NewRender2d),
    Canvas(NewRender2d, {100, 100}, {200, 200}, nullptr)
  {
    
  } /* End of 'test' function */

  VOID Init( VOID )
  {
    //Canvas.SetRoot(new ::ui::button({10, 10}));
    Canvas.SetRoot(new ::ui::div({0, 0}, Canvas.Size, {100, 100, 150}, {
      new ::ui::div({10, 10}, {100, 100}, {150, 200, 100}, {
          new ::ui::button({10, 10})
        }),
      new ::ui::div({110, 110}, {150, 150}, {150, 150, 150}, {
          new ::ui::button({10, 10})
        }),
      }));

    Canvas.Draw();
  } 

};

namespace tmp
{
  // scene class
  class scene
  {
  public:

    gfx Gfx;
    ::ui::render_2d Render2d;
    test Test;

    BOOL ButtonValue {1};

    /* Default scene construct function */
    scene( const HWND &hWnd, const INT &NewW, const INT &NewH ) :
      Gfx(hWnd, size(30, 30), ui::props::DefTextFormat),
      Render2d({NewW, NewH}),
      Test(Render2d)
    {
      Gfx.SetFrame(0xF00FF00F);
      Render2d.SetFrame(0xFF000000);

      Test.Init();
    } /* End of 'scene' function */

    /* Scene deconstruct function */
    ~scene()
    {
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
      static POINT OldCursorPos = {0, 0};
      static ui::CLICK_STATE OldMouseState = ui::CS_RELEASED;
      static ui::CLICK_STATE MouseState = ui::CS_RELEASED;
      
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

      Render2d.PutPixel({100, 100}, RGB(100, 200, 100));

      if (OldCursorPos.x != CursorPos.x || OldCursorPos.y != CursorPos.y)
        Test.Canvas.OnMouseMove({CursorPos.x - OldCursorPos.x, CursorPos.y - OldCursorPos.y}, {CursorPos.x, CursorPos.y});
      if (MouseState == ui::CS_CLICKED)
        Test.Canvas.OnMouseDown({CursorPos.x, CursorPos.y});
      if (MouseState == ui::CS_RELEASED && OldMouseState == ui::CS_PRESSED)
        Test.Canvas.OnMouseUp({CursorPos.x, CursorPos.y});

      OldCursorPos = CursorPos;
      OldMouseState = MouseState;
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

      if (Render2d.FrameSize.W != Fr.W || Render2d.FrameSize.H != Fr.H)
      {
        UINT
          TempW = std::min(Render2d.FrameSize.W, Fr.W),
          TempH = std::min(Render2d.FrameSize.H, Fr.H);
      
        for (UINT y = 0; y < TempH; y++)
          memcpy(&Fr.Image[Fr.W * y], &Render2d.Frame[y * Render2d.FrameSize.W], TempW * sizeof(DWORD));
      }
      else
      {
        memcpy(Fr.Image, Render2d.Frame.data(), Fr.W * Fr.H * sizeof(DWORD));
      }

      //if (Gfx.FrameSize.W != Fr.W || Gfx.FrameSize.H != Fr.H)
      //{
      //  INT TempW = std::min(Gfx.FrameSize.W, Fr.W), TempH = std::min(Gfx.FrameSize.H, Fr.H);
      //
      //  for (INT y = 0; y < TempH; y++)
      //    memcpy(&Fr.Image[Fr.W * y], &Gfx.Frame[y * Gfx.FrameSize.W], TempW * sizeof(DWORD));
      //}
      //else
      //{
      //  memcpy(Fr.Image, Gfx.Frame, Fr.W * Fr.H * sizeof(DWORD));
      //}
    } /* End of 'Render function'*/
  };
}

#endif //__rt_h_
