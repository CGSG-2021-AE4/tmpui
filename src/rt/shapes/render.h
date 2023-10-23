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

#include "./ui2/frame_render.h"
#include "./ui2/canvas.h"
#include "./ui2/controls.h"
#include "./ui2/controls/div.h"

#include <iostream>

/* Test class */
class test
{
public:

  ::ui::render_2d &Render2d;
  ::ui::canvas Canvas;

  /* Contructor function */
  test( ::ui::render_2d &NewRender2d, const ::ui::isize2 &Size ) :
    Render2d(NewRender2d),
    Canvas(NewRender2d, 0, Size, nullptr)
  {
  } /* End of 'test' function */

  VOID Init( VOID )
  {
    //Canvas.SetRoot(new ::ui::button({10, 10}));
    Canvas.SetRoot(new ::ui::controls::div("Canvas div", {0, 0}, Canvas.Size, {}, {
      new ::ui::controls::div("Div 1", {10, 10}, {100, 100}, {}, {
          new ::ui::button({10, 10})
        }),
      new ::ui::controls::div("Div 2", {110, 110}, {150, 150}, {}, {
          new ::ui::button({10, 10})
        }),
      }));

    Canvas.Draw();
  } 

};

namespace tmp
{
  enum struct mouse_state
  {
    NotPressed = 0,
    Released   = 1,
    Clicked    = 2,
    Pressed    = 3,
  }; /* End of 'mouse_state' enum struct*/

  // scene class
  class scene
  {
  public:

    HWND hWnd {nullptr};

    ::ui::render_2d Render2d;
    test Test;

    ivec2 OldMousePos {0, 0};
    mouse_state OldMouseState {mouse_state::Released};

    ivec2 GetMousePos( VOID )
    {
      POINT CursorPos;

      GetCursorPos(&CursorPos);

      ScreenToClient(hWnd, &CursorPos);

      return {CursorPos.x, CursorPos.y};
    } /* End of 'GetMousePos' function */

    /* Default scene construct function */
    scene( const HWND &NewhWnd, const INT &NewW, const INT &NewH ) :
      hWnd(NewhWnd),
      Render2d({NewW, NewH}),
      Test(Render2d, {NewW, NewH})
    {
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
      Render2d.Resize({NewW, NewH});
      Test.Canvas.Resize({NewW, NewH});
    } /* End of 'Resize' function */

    /* Ray tracing main render function
     * ARGUMENTS: None.
     * RETURNS: None
     */
    void Render( mth::cam<DBL> &Camera, frame &Fr )
    {
      mouse_state MouseState = mouse_state::NotPressed;
      
      if (GetKeyState(VK_LBUTTON) & 0x8000)
      {
        if (OldMouseState == mouse_state::Clicked)
          MouseState = mouse_state::Pressed;
        else if (OldMouseState != mouse_state::Pressed)
          MouseState = mouse_state::Clicked;
        else
          MouseState = mouse_state::Pressed;
      }
      else if (OldMouseState > mouse_state::Released)
        MouseState = mouse_state::Released;

      static INT OldFrameTime;
      static DBL FPS = 0;

      OldFrameTime = std::clock();

      ivec2 MousePos = GetMousePos();
      ivec2 DeltaMousePos = MousePos - OldMousePos;
      /*
       ************* DRAW INTERFACE BEGINNING *************
       */

      if (DeltaMousePos != ivec2(0, 0))
        Test.Canvas.OnMouseMove(DeltaMousePos, MousePos);
      if (MouseState == mouse_state::Clicked)
        Test.Canvas.OnMouseDown(MousePos);
      if (MouseState == mouse_state::Released)
        Test.Canvas.OnMouseUp(MousePos);

      /*
       ************* DRAW INTERFACE END *************
       */

      OldMousePos = MousePos;
      OldMouseState = MouseState;

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
    } /* End of 'Render function'*/
  };
}

#endif //__rt_h_
