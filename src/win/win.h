/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : win.h
 * PURPOSE     : Raytracing project.
 *               Windows dependent handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __win_h_
#define __win_h_

#include <windows.h>
#include <thread>
#include "../mth/mth.h"
#include "..\rt\shapes\render.h"
//#include "..\rt\shapes\shape.h"


#define WND_CLASS_NAME "AE4 programm"

namespace tmp
{
  class win
  {
  protected:
    HWND hWnd;           // window handle
    INT W, H;            // window size
    HINSTANCE hInstance; // application handle

    HDC hDC, hMemDC;
    HBITMAP hBm; // main bitmap
  

  private:
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );

    BOOL IsFullScreen;
    RECT FullScreenSaveRect;
    INT MouseWheel;
    

  public:
    BOOL IsActive;

    win( HINSTANCE hInst = GetModuleHandle(nullptr) ) : hInstance(hInst)
    {
      WNDCLASS wc;
      
      wc.style = CS_VREDRAW | CS_HREDRAW;
      wc.cbClsExtra = wc.cbWndExtra = 0;

      wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
      wc.hCursor = LoadCursor(NULL, IDC_CROSS);
      wc.hIcon = LoadIcon(NULL, IDI_HAND);
      wc.lpszMenuName = NULL;
      wc.hInstance = hInstance;
      wc.lpfnWndProc = WinFunc;
      wc.lpszClassName = WND_CLASS_NAME;
      wc.cbWndExtra = sizeof(win *);

      if (!RegisterClass(&wc))
      {
        MessageBox(NULL, "Error init", "ERROR", MB_OK);
        return;
      }

      hWnd = CreateWindow(WND_CLASS_NAME, "3D animation",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, reinterpret_cast<VOID *>(this));

      ShowWindow(hWnd,  SW_SHOWNORMAL);
      UpdateWindow(hWnd);
    }
    virtual ~win( VOID )
    {
    }
    
    virtual VOID Run( VOID );
    VOID FlipFullScreen( VOID );

  private:
    // message cracks
    VOID OnGetMinMaxInfo( MINMAXINFO *MinMax ); // ???
    virtual BOOL OnCreate( CREATESTRUCT *CS );
    virtual VOID OnDestroy( VOID );
    virtual BOOL OnEraseBkgnd( HDC hDC );
    virtual VOID OnPaint( HDC hDC, PAINTSTRUCT *Ps );
    virtual VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );
    virtual VOID OnKeyDown( UINT Keys );
    virtual VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );
    virtual VOID OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys );
    virtual VOID OnButtonUp( INT X, INT Y, UINT Keys );
    virtual VOID OnMouseMove( INT X, INT Y, UINT Keys );
    virtual VOID OnSize( UINT State, INT W, INT H );
    virtual VOID OnTimer( INT Id );
    virtual VOID OnDropFiles( HDROP hDrop );
    virtual VOID OnDrawItem( INT Id, DRAWITEMSTRUCT *DrawItem );
    virtual LRESULT OnNotify( INT Id, NMHDR *NoteHead );
    virtual VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify );
    virtual VOID OnMenuSelect( HMENU hMenu, INT Item, HMENU hMenuPopup, UINT Flags);
    virtual VOID OnInitMenuPopup( HMENU hMenu, UINT Item, BOOL IsSystemMenu );
    virtual VOID OnHScroll( HWND hWndCtl, UINT Code, INT Pos );
    virtual VOID OnVScroll( HWND hWndCtl, UINT Code, INT Pos );
    virtual VOID OnExitSizeMove( VOID );
    virtual VOID OnEnterSizeMove( VOID );
    virtual LRESULT OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam );
  };

  class rtwin : public win
  {
  public:
    scene StdScene;
    mth::cam<DBL> Camera;
    frame Fr;
    bool ResizeFlag = 0;

    //rtwin class construct
    rtwin( HINSTANCE hInst = GetModuleHandle(nullptr) ) : win(hInst), StdScene(hWnd, W, H), Camera()
    {
      Fr.Resize(W, H);
    }

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT W, INT H ) override
    {
      this->W = W;
      this->H = H;

      if (StdScene.Gfx.hBm != NULL)
        DeleteObject(StdScene.Gfx.hBm);
      
      hDC = GetDC(hWnd);
      
      StdScene.Gfx.hBm = CreateCompatibleBitmap(hDC, W, H);

      //ReleaseDC(hWnd, hDC);
      SelectObject(StdScene.Gfx.hMemDC, StdScene.Gfx.hBm);

      Fr.Resize(W, H);
      Camera.Resize(W, H);
      StdScene.Gfx.Resize(size(W, H));
      ResizeFlag = 1;
    } /* End of 'win::OnSize' function */

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    VOID OnPaint( HDC DrawhDC, PAINTSTRUCT *PS ) override
    {
      static unsigned long long FrameCounter = 0;
      //HPEN hPn = (HPEN)GetStockObject(DC_PEN);
      //HBRUSH hBr = (HBRUSH)GetStockObject(DC_BRUSH);
      //
      //SelectObject(hMemDC, hBr);
      //SetDCBrushColor(hMemDC, RGB(0, 25, 0));
      //SelectObject(hMemDC, hPn);
      //SetDCPenColor(hMemDC, RGB(0, 25, 0));
      //Rectangle(hMemDC, 0, 0, W, H);
      //SetDCPenColor(hMemDC, RGB(255, 255, 255));
      ///* Paint */
      //
      //
      //SetDCPenColor(hMemDC, RGB(0, 255, 0));
      ////PutPixel(hMemDC, ScaleXC, ScaleYC);
      //SetPixel(hMemDC, 100, 100, RGB(255, 0, 0));
  
  
  
      /*
      DWORD *ImageBits = new DWORD(ImageBm.bmWidth * ImageBm.bmHeight);
  
      if (GetBitmapBits(hImageBm, ImageBm.bmWidth * ImageBm.bmHeight, ImageBits))
      {
        BITMAPINFOHEADER bmih = {sizeof(BITMAPINFOHEADER ), ImageBm.bmWidth, -ImageBm.bmHeight, ImageBm.bmPlanes, ImageBm.bmBitsPixel};
        SetDIBits(hMemDC, hBm, 0, ImageBm.bmHeight, ImageBits, (BITMAPINFO *)&bmih, DIB_RGB_COLORS);
      }
      */

      if (GetAsyncKeyState(VK_UP) & 0x8000)
        Camera.Rotate(vec3(-1, 0, 0));
      if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        Camera.Rotate(vec3(0, -1, 0));
      if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        Camera.Rotate(vec3(1, 0, 0));
      if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        Camera.Rotate(vec3(0, 1, 0));

        //Camera movement
      if (GetAsyncKeyState('W') & 0x8000)
        Camera.Move(vec3(0, 0, 0.2));
      if (GetAsyncKeyState('A') & 0x8000)
        Camera.Move(vec3(-0.2, 0, 0));
      if (GetAsyncKeyState('S') & 0x8000)
        Camera.Move(vec3(0, 0, -0.2));
      if (GetAsyncKeyState('D') & 0x8000)
        Camera.Move(vec3(0.2, 0, 0));

      if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        Camera.Move(vec3(0, 0.2, 0));
      if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
        Camera.Move(vec3(0, -0.2, 0));

      
      Fr.Display(StdScene.Gfx.hMemDC, StdScene.Gfx.hBm);
      
      if (!ResizeFlag)
        StdScene.Render(Camera, Fr);
      ResizeFlag = 0;

         
      //StdScene.Render(Camera, Fr);
  
      StdScene.Gfx.RenderFrame();

      FrameCounter++;
    } /* End of 'win::OnPaint' function */

    /* Key down message reponse function function.
     * ARGUMENTS:
     *   - key index:
     *       UINT Keys;
     * RETURNS: None.
     */
    VOID OnKeyDown( UINT Keys ) override
    {
      switch (Keys)
      {
        /*
        //Camera rotation 
      case VK_UP:
        Camera.Rotate(vec3(-4, 0, 0));
        break;
      case VK_LEFT:
        Camera.Rotate(vec3(0, -4, 0));
        break;
      case VK_DOWN:
        Camera.Rotate(vec3(4, 0, 0));
        break;
      case VK_RIGHT:
        Camera.Rotate(vec3(0, 4, 0));
        break;
        //Camera movement
      case 'W':
        Camera.Move(vec3(0, 0, 1));
        break;
      case 'A':
        Camera.Move(vec3(-1, 0, 0));
        break;
      case 'S':
        Camera.Move(vec3(0, 0, -1));
        break;
      case 'D':
        Camera.Move(vec3(1, 0, 0));
        break;
      case VK_SPACE:
        Camera.Move(vec3(0, 1, 0));
        break;
      case VK_SHIFT:
        Camera.Move(vec3(0, -1, 0));
        break;*/

        //Flip full screen
      case 'F':
        FlipFullScreen();
        break;
      case VK_ESCAPE:
        DestroyWindow(hWnd);
        break;

      case 'C':
      Fr.Save("Shoot");
      break;
      case 'X':
        if (1)
        {
          POINT P;
          GetCursorPos(&P);
          INT
            x = P.x,
            y = P.y;
          x = y;
        }
        break;
      }
    } /* End of 'OnKeyDown' function */
  };
}

#endif //__win_h_

/* END OF 'win.h' FILE */