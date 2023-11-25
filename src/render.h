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
#include "./frame/frame.h"
#include "./mth/mth.h"
#include <vector>
#include "def.h"

#define BACK_GRD_COLOR RGB(204, 120, 77)
#define BACK_GRD_COLOR_VEC vec3(0.30, 0.47, 0.8)
#define RT_THESHOLD 0.001

#include "./ui/frame_render.h"
#include "./ui/canvas.h"

// Examples
#include "./ui/examples/default.h"
#include "./ui/examples/chat.h"
#include "./ui/examples/chat_server.h"


#include <iostream>

namespace cs = ::ui::controls;

/* Test class */
class test
{
public:
  
  // ::ui::box_style StdDivStyle { .Space = { .DefColor = 0.35 }, .Border = { .DefColor = 0.75 } };

  ::ui::box_props StdDivBoxProps = ::ui::box_props({ .MarginW = 4, .BorderW = 2, .PaddingW = 2 });
  ::ui::box_props StdMarginBoxProps = ::ui::box_props({ .MarginW = 2, .BorderW = 0, .PaddingW = 0 });
  cs::div_style StdDivStyle = cs::div_style({ .SpaceColor = {0.35}, .BorderColor = {0.75} });


  ::ui::render_2d &Render2d;
  ::ui::canvas Canvas;

  //std::vector<cs::slider *> Sliders;

  /* Contructor function */
  test( ::ui::render_2d &NewRender2d, const ::ui::isize2 &Size ) :
    Render2d(NewRender2d),
    Canvas(NewRender2d, 0, Size, ::ui::layout_type::eFlexRow, {})
  { 
    Canvas.GetRoot()->AddChildren({
      ::ui::Create<::ui::components::tabs>({
        .Flex = { .Grow = 1, .Shrink = 1 },
        .Tabs = {
          { "First example",
            ::ui::Create<::ui::examples::def>({
              .Flex = { .Grow = 1, .Shrink = 1 },
            })
          },
          { "Chat",
            ::ui::Create<::ui::examples::chat>({
              .Flex = { .Grow = 1, .Shrink = 1 },
            })
          },
          { "Chat server",
            ::ui::Create<::ui::examples::chat_server>({
              .Flex = { .Grow = 1, .Shrink = 1 },
            })
          },
        }
      }) // Main tabs
    }); 

  } /* End of 'test' function */

  VOID Init( VOID )
  {
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
      auto Start = std::chrono::high_resolution_clock::now();
      Test.Canvas.Resize({NewW, NewH});
      auto End = std::chrono::high_resolution_clock::now();
      std::cout << std::format("Resize time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(End - Start));
    } /* End of 'Resize' function */

    VOID OnWheel( INT WheelDelta )
    {
      ivec2 MousePos = GetMousePos();

      auto Start = std::chrono::high_resolution_clock::now();
      Test.Canvas.OnMouseMove({0, 0, WheelDelta / 5}, MousePos);
      auto End = std::chrono::high_resolution_clock::now();
      std::cout << std::format("On mouse wheel time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(End - Start));
    } /* End of 'OnWheel' function */

    /* Ray tracing main render function
     * ARGUMENTS:
     *   - frame to draw:
     *       frame &Fr;
     * RETURNS: None
     */
    void Render( frame &Fr )
    {
      static ULONGLONG FrameCounter = 0;
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

      ivec2 MousePos = GetMousePos();
      ivec2 DeltaMousePos = MousePos - OldMousePos;
      /*
       ************* DRAW INTERFACE BEGINNING *************
       */
      std::chrono::steady_clock::time_point T1, T2;

      // T1 = std::chrono::high_resolution_clock::now();

      if (DeltaMousePos != ivec2(0, 0))
        Test.Canvas.OnMouseMove({DeltaMousePos.X, DeltaMousePos.Y, 0}, MousePos);
      if (MouseState == mouse_state::Clicked)
        Test.Canvas.OnMouseDown(MousePos);
      if (MouseState == mouse_state::Released)
        Test.Canvas.OnMouseUp(MousePos);

      // T2 = std::chrono::high_resolution_clock::now();

      // std::cout << std::format("Response time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(T2 - T1));

      // for (UINT i = 0; i < (UINT)Test.Sliders.size(); i++)
      //   Test.Sliders[i]->SetValue(abs(sin(sin(i) + FrameCounter * 0.1)));

      T1 = std::chrono::high_resolution_clock::now();
      //st.Render2d.FillBar(0, Text.);
      //Test.Render2d.ClearFrame();
      Test.Canvas.Draw();
      
      T2 = std::chrono::high_resolution_clock::now();

      std::cout << std::format("Draw time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(T2 - T1));
      
      FrameCounter++;
      /*
       ************* DRAW INTERFACE END *************
       */

      OldMousePos = MousePos;
      OldMouseState = MouseState;

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
