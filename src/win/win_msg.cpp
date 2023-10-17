/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : win_msg.cpp
 * PURPOSE     : Raytracing project.
 *               Windows messages module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __win_msg_cpp_
#define __win_msg_cpp_

#include "win.h"
#include "../mth/mth.h"


/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - min max info struct :
 *       MINMAXINFO *MinMax;
 * RETURNS: None.
 */
VOID tmp::win::OnGetMinMaxInfo( MINMAXINFO *MinMax )
{
  MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    MinMax->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) * 8 +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    MinMax->ptMinTrackSize.x += 300;
}; /* End of 'OnGetMinMaxInfo' function */

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL tmp::win::OnCreate( CREATESTRUCT *CS )
{

  hDC = GetDC(hWnd);
  //hMemDC = CreateCompatibleDC(hDC);
  hMemDC = 0;
  ReleaseDC(hWnd, hDC);
  SetTimer(hWnd, 30, 1, NULL);
  
  return TRUE;
} /* End of 'win::OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tmp::win::OnDestroy( VOID )
{
  DeleteObject(hBm);
  DeleteDC(hMemDC);
  KillTimer(hWnd, 30);
  PostQuitMessage(0);
} /* End of 'win::OnDestroy' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
VOID tmp::win::OnSize( UINT State, INT W, INT H )
{
  this->W = W;
  this->H = H;
  if (hBm != NULL)
    DeleteObject(hBm);
  hDC = GetDC(hWnd);
  hBm = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(hWnd, hDC);
  SelectObject(hMemDC, hBm);
} /* End of 'win::OnSize' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
BOOL tmp::win::OnEraseBkgnd( HDC hDC )
{
  return 0;
} /* End of 'win::OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window device context:
 *       HDC hDC;
 *   - paint message structure pointer:
 *       PAINTSTRUCT *PS;
 * RETURNS: None.
 */
VOID tmp::win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
} /* End of 'win::OnPaint' function */ 


/* WM_ACTIVATE window message handle function.
 * ARGUMENTS:
 *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
 *       UINT Reason;
 *   - handle of active window:
 *       HWND hWndActDeact;
 *   - minimized flag:
 *       BOOL IsMinimized;
 * RETURNS: None.
 */
VOID tmp::win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
{
   IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
} /* End of 'win::OnActivate' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID tmp::win::OnTimer( INT Id ) 
{
  InvalidateRect(hWnd, NULL, 1);
} /* End of 'win::OnTimer' function */

/* WM_MOUSEWHEEL window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse wheel relative delta value:
 *       INT Z;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tmp::win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  MouseWheel += Z;
} /* End of 'win::OnMouseWheel' function */


/* Key down message reponse function function.
 * ARGUMENTS:
 *   - key index:
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tmp::win::OnKeyDown( UINT Keys )
{
} /* End of 'tmp::win::OnKeyDown' function */

/* Mouse button down message reponse function function.
 * ARGUMENTS:
 *   - double click check flag:
 *       BOOL IsDoubleClick;
 *   - mouse position:
 *       INT x, y;
 *   - key index:
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tmp::win::OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys )
{
  
} /* End of 'tmp::win::OnButtonDown' function */
/* Mouse button down message reponse function function.
 * ARGUMENTS:
 *   - mouse position:
 *       INT x, y;
 *   - key index:
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tmp::win::OnButtonUp( INT X, INT Y, UINT Keys )
{
} /* End of 'tmp::win::OnButtonUp' function */
/* Mouse move message reponse function function.
 * ARGUMENTS:
 *   - mouse position:
 *       INT x, y;
 *   - key index:
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tmp::win::OnMouseMove( INT X, INT Y, UINT Keys )
{

} /* End of 'tmp::win::OnMouseMove' function */

/* WM_DROPFILES window message handle function.
 * ARGUMENTS:
 *   - handle to an internal structure describing the dropped files:
 *       HDROP hDrop;
 * RETURNS: None.
 */
VOID tmp::win::OnDropFiles( HDROP hDrop )
{
  static CHAR Buf[_MAX_PATH];

  INT n = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
  if (n > 0)
  {
    POINT pt;
    DragQueryPoint(hDrop, &pt);
    ClientToScreen(hWnd, &pt);
    HWND hWndItem = WindowFromPoint(pt);
    if (hWndItem == hWnd )
    {
      /* for single file: DragQueryFile(hDrop, 0, Buf, sizeof(Buf)); */
      // Handle every file
      for (INT i = 0; i < n; i++)
      {
        DragQueryFile(hDrop, i, Buf, sizeof(Buf));
        /// Handle filename from 'Buf'
        // Can split:
        static CHAR
          Drive[_MAX_DRIVE], Dir[_MAX_DIR],
          FileName[_MAX_FNAME], FileExt[_MAX_EXT];
        _splitpath(Buf, Drive, Dir, FileName, FileExt);
        // . . .
      }
    }
  }
} /* End of 'win::OnDropFiles' function */

/* WM_NOTIFY window message handle function.
 * ARGUMENTS:
 *   - control identifier:
 *       INT Id;
 *   - notification header depended to control type:
 *       NMHDR *NoteHead;
 * RETURNS:
 *   (LRESULT) return value depended to notification.
 */
LRESULT tmp::win::OnNotify( INT Id, NMHDR* NoteHead )
{
  return 0;
} /* End of 'win::OnNotify' function */

/* WM_COMMAND window message handle function.
 * ARGUMENTS:
 *   - item (menu or accelerator) or control window identifier:
 *       INT Id;
 *   - control window handle:
 *       HWND hWndCtl;
 *   - notification: 0 - menu, 1 - accelerator, otherwise -
 *     depended to control (like BM_CLICKED):
 *       UINT CodeNotify;
 * RETURNS: None.
 */
VOID tmp::win::OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify )
{
} /* End of 'win::OnCommand' function */


/* WM_DRAWITEM window message handle function.
 * ARGUMENTS:
 *   - control identifier (for menu - 0):
 *       INT Id;
 *   - draw item information structure pointer:
 *       DRAWITEMSTRUCT *DrawItem;
 * RETURNS: None.
 */
VOID tmp::win::OnDrawItem( INT Id, DRAWITEMSTRUCT* DrawItem )
{
  /*
  if (DrawItem->CtlID == ...) // sample for check bax draw as button
  {
    UINT Flags = DFCS_BUTTONPUSH;

    if (DrawItem->itemState & ODS_DISABLED)
      Flags |= DFCS_INACTIVE;
    if (DrawItem->itemState & ODS_SELECTED)
      Flags |= DFCS_PUSHED;

    DrawFrameControl(DrawItem->hDC, &DrawItem->rcItem, DFC_BUTTON, Flags);
    InflateRect(&DrawItem->rcItem, -2, -2);
    Flags &= ~DFCS_PUSHED;
    Flags &= ~DFCS_BUTTONPUSH;
    Flags |= DFCS_BUTTONCHECK | DFCS_FLAT;
    if (DrawItem->CtlID == ...)
      Flags |= DFCS_CHECKED;
    DrawFrameControl(DrawItem->hDC, &DrawItem->rcItem, DFC_BUTTON, Flags);
  }
  */
} /* End of 'win::OnDrawItem' function */

/* WM_HSCROLL message handle function.
 * ARGUMENTS:
 *   - handle of scroll window or nullptr for window scroll bars:
 *       HWND hWndCtl;
 *   - scrool bar request code (see SB_***):
 *       UINT Code;
 *   - schroll position for 'Code' is SB_THUMBPOSITION or SB_THUMBTRACK:
 *       INT Pos;
 * RETURNS: None.
 */
VOID tmp::win::OnHScroll( HWND hWndCtl, UINT Code, INT Pos )
{
} /* End of 'win::OnHScroll' function */

/* WM_VSCROLL message handle function.
 * ARGUMENTS:
 *   - handle of scroll window or nullptr for window scroll bars:
 *       HWND hWndCtl;
 *   - scrool bar request code (see SB_***):
 *       UINT Code;
 *   - schroll position for 'Code' is SB_THUMBPOSITION or SB_THUMBTRACK:
 *       INT Pos;
 * RETURNS: None.
 */
VOID tmp::win::OnVScroll( HWND hWndCtl, UINT Code, INT Pos )
{
} /* End of 'win::OnVScroll' function */


/* WM_MENUSELECT window message handle function.
 * ARGUMENTS:
 *   - handle of menu:
 *       HMENU hMenu;
 *   - submenu item ID or submenu index
 *       INT Item;
 *   - handle of popup menu if selected popup submenu:
 *       HMENU hMenuPopup;
 *   - menu item flags (see MF_***):
 *       UINT Flags;
 * RETURNS: None.
 */
VOID tmp::win::OnMenuSelect( HMENU hMenu, INT Item, HMENU hMenuPopup, UINT Flags )
{
} /* End of 'win::OnMenuSelect' function */

/* WM_INITMENUPOPUP window message handle function.
 * ARGUMENTS:
 *   - handle of popup menu:
 *       HMENU hMenu;
 *   - submenu item position:
 *       UINT Item;
 *   - window system menu flag:
 *       BOOL IsSystemMenu;
 * RETURNS: None.
 */
VOID tmp::win::OnInitMenuPopup( HMENU hMenu, UINT Item, BOOL IsSystemMenu )
{
} /* End of 'win::OnInitMenuPopup' function */

/* WM_EXITSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tmp::win::OnExitSizeMove( VOID )
{
} /* End of 'win::OnExitSizeMove' function */

/* WM_ENTERSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tmp::win::OnEnterSizeMove( VOID )
{
} /* End of 'win::OnEnterSizeMove' function */

/* Window message universal handle function.
 * Should be returned 'DefWindowProc' call result.
 * ARGUMENTS:
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT tmp::win::OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'win::OnMessage' function */




#endif //__win_msg_cpp_