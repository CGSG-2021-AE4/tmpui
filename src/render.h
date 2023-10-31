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
#include "./ui/controls/div.h"
#include "./ui/controls/button.h"
#include "./ui/controls/slider.h"
#include "./ui/controls/text.h"

#include <iostream>

namespace cs = ::ui::controls;

/* Test class */
class test
{
public:
  
  ::ui::box_props StdDivProps { .MarginW = 4, .BorderW = 2, .PaddingW = 2 };
  cs::div_style StdDivStyle { .SpaceColor = {0.35}, .BorderColor = {0.75} };
  //::ui::box_style StdDivStyle { .Space = { .DefColor = 0.35 }, .Border = { .DefColor = 0.75 } };

  ::ui::render_2d &Render2d;
  ::ui::canvas Canvas;
  std::vector<cs::slider *> Sliders;

  /* Contructor function */
  test( ::ui::render_2d &NewRender2d, const ::ui::isize2 &Size ) :
    Render2d(NewRender2d),
    Canvas(NewRender2d, 0, Size, { .Type = ::ui::layout_type::eFlexRow }, {})
  {
    std::vector<::ui::entity *> ListDivs;

    for (UINT i = 0; i < 30; i++)
    {
      auto *SliderPtr = new cs::slider({
            .Id = std::format("Slider 1.2.{}.2.1", i + 1),
            .Pos = {0},
            .Size = {200, 30},
            .Style = { .Track = { .Space = { .DefColor = ::ui::vec3::Rnd0() }, .Border = { .DefColor = ::ui::vec3::Rnd0() } },
                       .Thumb = { .Space = { .DefColor = ::ui::vec3::Rnd0() }, .Border = { .DefColor = ::ui::vec3::Rnd0() } }},
            });

      Sliders.push_back(SliderPtr);

      ListDivs.push_back(new cs::div({
        .Id = std::format("Div 1.2.{}", i + 1),
        .LayoutProps = { .Type = ::ui::layout_type::eFlexRow, .Flex = 0, .MinSize = 100 },
        .BoxProps = StdDivProps,
        .Style = { .SpaceColor = ::ui::vec3::Rnd0(), .BorderColor = ::ui::vec3::Rnd0() }
        }, {
        new cs::div({
          .Id = std::format("Div 1.2.{}.1", i + 1),
          .LayoutProps = { .Flex = 0, .MinSize = 30 },
          .BoxProps = StdDivProps,
          .Style = { .SpaceColor = ::ui::vec3::Rnd0(), .BorderColor = ::ui::vec3::Rnd0() }
          }),
        new cs::div({
          .Id = std::format("Div 1.2.{}.2", i + 1),
          .LayoutProps = { .Flex = 1, .MinSize = 30 },
          .BoxProps = StdDivProps,
          .Style = { .SpaceColor = ::ui::vec3::Rnd0(), .BorderColor = ::ui::vec3::Rnd0() }
          }, {
            SliderPtr,
          }),
        new cs::button({
          .Id = std::format("Button 1.2.{}.3", i + 1),
          .LayoutProps = { .Flex = 0, .MinSize = 30, .MaxSize = 30 },
          .IsPress = true,
          }),
        }));
    }

    Canvas.GetRoot()->AddChildren({
      new cs::div({ .Id = "Left bar", .LayoutProps = { .MinSize = {30} }, .BoxProps = { .MarginW = 0, .BorderW = 2, .PaddingW = 2 }, .Style = StdDivStyle }, {}),
      new cs::div({ .Id = "Div 1", .LayoutProps = { .Type = ::ui::layout_type::eFlexRow, .Flex = 1 }, .BoxProps = { .MarginW = 0, .BorderW = 0, .PaddingW = 2 }, .Style = StdDivStyle }, {
        new cs::div({ .Id = "Div 1.1", .LayoutProps = { .Type = ::ui::layout_type::eFlexRow, .Flex = 1 }, .BoxProps = StdDivProps, .Style = StdDivStyle }, {
          new cs::div({ .Id = "Div 1.1.1", .LayoutProps = { .Type = ::ui::layout_type::eFlexRow, .Flex = 2 }, .BoxProps = StdDivProps, .Style = { .SpaceColor = ::ui::vec3::Rnd0(), .BorderColor = ::ui::vec3::Rnd0() } }, {
            new cs::text({ .Id = "Text 1.1.1.1", .LayoutProps = { .Flex = 1 }, .Style = { .Color = 0 }, .Str =
              ("Hi, my dear fried. Here there is a small description of my user interface.\nMy user interface has different props:\n'Div' - it is a box control, analog of html div. It has just space and border color which can be set by the user.\n'Button' - simple press button.\n'Slider' - now it is just a slider that changes value from 0 to 1.\n'Text' - just text you see.\n\nThank you for reading this shit. :)")}),
            }),
          new cs::div({ .Id = "Div 1.1.2", .LayoutProps = { .Flex = 1 }, .BoxProps = StdDivProps, .Style = { .SpaceColor = ::ui::vec3::Rnd0(), .BorderColor = ::ui::vec3::Rnd0() } }, {}),
          }),
        new cs::div({ .Id = "Div 1.2", .LayoutProps = { .Type = ::ui::layout_type::eFlexColumn, .Flex = 1, .IsScrollable = true }, .BoxProps = StdDivProps, .Style = StdDivStyle }, ListDivs),
        }),
      new cs::div({ .Id = "Right bar", .LayoutProps = { .MinSize = {30} }, .BoxProps = { .MarginW = 0, .BorderW = 2, .PaddingW = 2 }, .Style = StdDivStyle }, {}),
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
      // auto Start = std::chrono::high_resolution_clock::now();
      Test.Canvas.OnMouseMove({0, 0, WheelDelta / 5}, MousePos);
      // auto End = std::chrono::high_resolution_clock::now();
      // std::cout << std::format("On mouse wheel time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(End - Start));
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

      //std::cout << std::format("Response time {} microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(T2 - T1));

      //for (UINT i = 0; i < (UINT)Test.Sliders.size(); i++)
      //  Test.Sliders[i]->SetValue(abs(sin(sin(i) + FrameCounter * 0.1)));

      T1 = std::chrono::high_resolution_clock::now();
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
