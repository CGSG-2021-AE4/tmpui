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
// #include "./ui/controls/slider.h"
#include "./ui/controls/text.h"
// #include "./ui/controls/range.h"

#include <iostream>

namespace cs = ::ui::controls;

template<typename entity_type>
  ::ui::entity * Create( const ::ui::entity_props<entity_type> &Props, const std::vector<::ui::entity *> &NewChildren = {}, ::ui::entity *NewParent = nullptr )
  {
    ::ui::entity * NewE = new entity_type(Props);

    NewE->AddChildren(NewChildren);
    NewE->SetParent(NewParent);
    return NewE;
  } /* End of 'Create' function */

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

  /* Create value function - test component */
  ::ui::entity * CreateValue( const std::string &Name )
  {
    std::string Prefix = std::format("Value {} ", Name);

    return
      Create<cs::div>({
        .Id = Prefix + "main bar",
        .LayoutType = ::ui::layout_type::eFlexRow,
        .BoxProps = StdDivBoxProps,
        .Style = StdDivStyle
      }, {
        Create<cs::text>({
          .Id = Prefix + "second button bar text",
          .Flex = { .Grow = 1 },
          .BoxProps = StdDivBoxProps,
          .Props = { .Str = "Button of value " + Name },
          .Style = { .Color = ::ui::vec3(1) }
        }),
        Create<cs::button>({
          .Id = "second button bar button",
          .Size = 30, 
          .Flex = { .Basis = ::ui::flex_basis_type::eFixed },
          .BoxProps = StdDivBoxProps,
          .Props = { .IsPress = true, .OnChangeCallBack = []( cs::button *Button ){ ::ui::Log(std::format("New value: {}", Button->GetValue() )); } },
        }),
      }); // Main bar
  } /* End of 'CreateValue' function */

  //std::vector<cs::slider *> Sliders;

  /* Contructor function */
  test( ::ui::render_2d &NewRender2d, const ::ui::isize2 &Size ) :
    Render2d(NewRender2d),
    Canvas(NewRender2d, 0, Size, ::ui::layout_type::eFlexRow, {})
  {
    std::vector<::ui::entity *> Values;
    
    for (UINT i = 0; i < 30; i++)
      Values.push_back(CreateValue(std::format("{}", i)));

    Canvas.GetRoot()->AddChildren({
      Create<cs::div>({
        .Id = "Main bar",
        .MaxSize = {5000},
        .LayoutType = ::ui::layout_type::eFlexRow,
        .Flex = { .Grow = 1, .Shrink = 1 },
        .BoxProps = StdDivBoxProps,
        .Style = StdDivStyle
      }, {
        Create<cs::div>({
          .Id = "Box 1",
          .Size = {300},
          //.MinSize = {200},
          .MaxSize = {1000},
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
          .BoxProps = StdDivBoxProps,
          .Style = StdDivStyle,
        }, {
          Create<cs::div>({
            .Id = "Box 1.1",
            .Size = {300},
            .MinSize = {270},
            .MaxSize = {500},
            .Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
            .BoxProps = StdDivBoxProps,
            .Style = StdDivStyle,
          }),
          Create<cs::div>({
            .Id = "Box 1.1",
            .Size = {300},
            //.MinSize = {270},
            .MaxSize = {500},
            .LayoutType = ::ui::layout_type::eFlexColumn,
            .Overflow = ::ui::overflow_type::eScroll,
            .Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
            .BoxProps = StdDivBoxProps,
            .Style = StdDivStyle,
          }, Values),
        }),
        Create<cs::div>({
          .Id = "Box 2",
          .Size = {300},
          .MinSize = {270},
          .MaxSize = {600},
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 2, .Shrink = 2 },
          .BoxProps = StdDivBoxProps,
          .Style = StdDivStyle,
        }),
      }),
    });

    /*
    std::vector<::ui::entity *> Values;

    for (UINT i = 0; i < 30; i++)
      Values.push_back(CreateValue(std::format("{}", i)));

    Canvas.GetRoot()->AddChildren({
      Create<cs::div, ::ui::entity_props<BYTE, cs::div_style>>({
        .Id = "Main bar",
        .Pos = {100},
        .Size = {300, 200},
        .LayoutType = ::ui::layout_type::eFlexRow,
        .Flex = { .Grow = 1, .Shrink = 1 },
        .BoxProps = StdDivBoxProps,
        .Style = StdDivStyle
      }, {
        Create<cs::div, ::ui::entity_props<BYTE, cs::div_style>>({
          .Id = "Value list",
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Overflow = ::ui::overflow_type::eScroll,
          .Flex = { .Grow = 2, .Shrink = 1 },
          .BoxProps = StdDivBoxProps,
          .Style = StdDivStyle,
        }, Values),
        Create<cs::text, ::ui::entity_props<cs::text_props, cs::text_style>>({
          .Id = "Text2",
          .Flex = { .Grow = 1 },
          .BoxProps = StdDivBoxProps,
          .Props = {
            .Str = "Some text" //"I pounder of something great, my lungs will fill and then deflate. They fill with fire, exhale desire, I know it's dire my time today."
          },
          .Style = { .Color = ::ui::vec3(1) } }),
        Create<cs::div, ::ui::entity_props<BYTE, cs::div_style>>({
          .Id = "Button bar",
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Overflow = ::ui::overflow_type::eScroll,
          .BoxProps = StdDivBoxProps,
          .Style = StdDivStyle,
        }, {
          Create<cs::div, ::ui::entity_props<BYTE, cs::div_style>>({
            .Id = "Button bar 1",
            .LayoutType = ::ui::layout_type::eFlexRow,
            .BoxProps = StdDivBoxProps,
            .Style = StdDivStyle,
          }, {
            Create<cs::button, ::ui::entity_props<cs::button_props, cs::button_style>>({
              .Id = "Button 1.1",
              .Size = 30, 
              .Flex = { .Basis = ::ui::flex_basis_type::eFixed },
              .BoxProps = StdDivBoxProps,
              .Props = { .IsPress = true, .OnChangeCallBack = []( cs::button *Button ){ ::ui::Log(std::format("New value: {}", Button->GetValue() )); } },
            }),
            Create<cs::button, ::ui::entity_props<cs::button_props, cs::button_style>>({
              .Id = "Button 1.2",
              .Size = 30, 
              .Flex = { .Basis = ::ui::flex_basis_type::eFixed },
              .BoxProps = StdDivBoxProps,
              .Props = { .IsPress = true, .OnChangeCallBack = []( cs::button *Button ){ ::ui::Log(std::format("New value: {}", Button->GetValue() )); } },
            }),
          }),
          Create<cs::button, ::ui::entity_props<cs::button_props, cs::button_style>>({
            .Id = "Button 1",
            .Size = 30, 
            .Flex = { .Basis = ::ui::flex_basis_type::eFixed },
            .BoxProps = StdDivBoxProps,
            .Props = { .IsPress = true, .OnChangeCallBack = []( cs::button *Button ){ ::ui::Log(std::format("New value: {}", Button->GetValue() )); } },
            }),
          Create<cs::button, ::ui::entity_props<cs::button_props, cs::button_style>>({
            .Id = "Button 2",
            .Size = 30,
            .Flex = { .Basis = ::ui::flex_basis_type::eFixed },
            .BoxProps = StdDivBoxProps,
            .Props = { .IsPress = true, .OnChangeCallBack = []( cs::button *Button ){ ::ui::Log(std::format("New value: {}", Button->GetValue() )); } },
          }),
          Create<cs::slider, cs::slider_props>({
            .Id = "Slider 1",
            .Size = {200, 30},
            .Style = {
              .Track = { .Space = { .DefColor = ::ui::vec3::Rnd0() }, .Border = { .DefColor = ::ui::vec3::Rnd0() } },
              .Thumb = { .Space = { .DefColor = ::ui::vec3::Rnd0() }, .Border = { .DefColor = ::ui::vec3::Rnd0() } }
            },
          }),
        }), // Button bar
      }), // Main bar
    });

    */
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
