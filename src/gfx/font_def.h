/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : font_def.h
  * PURPOSE     : Skinning project.
  *               Main font handle module.
  * PROGRAMMER  : CGSG-FallCamp'2022.
  *               Andrey Egorov.
  * LAST UPDATE : 24.11.2022.
  * NOTE        : Module namespace 'tmp'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <def.h>
#include <cmath>

#ifndef __font_def_h_
#define __font_def_h_

//#include <tmp.h>

namespace tmp
{
  // Main ui font class
  class font
  {
  public:

    typedef enum
    {
      TAH_LEFT   = 0,          // 000000
      TAH_CENTER = 1,          // 000001
      TAH_RIGHT  = 2,          // 000010
      TAH_MASK   = 3           // 000011
    } text_align_hor;

    typedef enum
    {
      TAV_TOP   = 0 << 2,      // 000000
      TAV_CENTER = 1 << 2,     // 000100
      TAV_BOTTOM  = 2 << 2,    // 001000
      TAV_MASK   = 3 << 2      // 001100
    } text_align_vert;

    typedef enum
    {
      TAP_CLIP = 1 << 4,       // 010000
      TAP_SINGLELINE = 1 << 5, // 100000
    } text_align_props;

    HDC hDC {};
    HDC hMemDC {};
    HBITMAP hBm;
    DWORD TextColor {};
    INT TextFormat {};
    HFONT hFont;


    /* 'font' def construct function.
      * ARUGMENTS:
      *   - window handle:
      *       const HWND &hWnd;
      *   - text h:
      *       const INT &FontH;
      *   - text draw format:
      *       const BYTE &NewDrawFromat;
      *   - text color:
      *       const DWORD &NewTextColor;
      * RETURNS: None.
      */
    font( const HWND &hWnd, const INT &FontH, const BYTE &NewTextFormat, const DWORD &NewTextColor = 0xFFFFFFFF ) : TextColor(NewTextColor), TextFormat(0),
      hFont(CreateFont(FontH, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
                       OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                       DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas")))
    {
      // Setup draw handles
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

      RECT ClientRect;
        
      GetClientRect(hWnd, &ClientRect);
      hBm = CreateCompatibleBitmap(hDC, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);
      SelectObject(hMemDC, hBm);

      // Setpu font
      SelectObject(hMemDC, hFont);
      SetTextColor(hMemDC, TextColor);
      SetBkMode(hMemDC, TRANSPARENT);
        
      // Horizontal alignment
      switch (NewTextFormat & TAH_MASK)
      {
      case TAH_LEFT:
        TextFormat |= DT_LEFT;
        break;
      case TAH_CENTER:
        TextFormat |= DT_CENTER;
        break;
      case TAH_RIGHT:
        TextFormat |= DT_RIGHT;
        break;
      }

      // Vertical alignment
      switch (NewTextFormat & TAV_MASK)
      {
      case TAV_TOP:
        TextFormat |= DT_TOP;
        break;
      case TAV_CENTER:
        TextFormat |= DT_VCENTER;
        break;
      case TAV_BOTTOM:
        TextFormat |= DT_BOTTOM;
        break;
      }

      // Other props
      if (!(NewTextFormat & TAP_CLIP))
        TextFormat |= DT_NOCLIP;
      if (NewTextFormat & TAP_SINGLELINE)
        TextFormat |= DT_SINGLELINE;

    } /* End of 'font' function */

    /* Put text function.
     * ARGUMENTS:
     *   - drawing string:
     *       const std::string &Str;
     *   - textx position:
     *       const ivec2 &Pos;
     *   - text size:
     *       const size &TextSize;
     * RETURNS: None.
     */
    VOID PutText( const std::string &Str, const ivec2 &Pos, const size &TextSize )
    {
      RECT Rect = {Pos.X, Pos.Y, Pos.X + TextSize.W, Pos.Y + TextSize.H};

      DrawText(hMemDC, Str.c_str(), -1, &Rect, TextFormat);
    } /* End of 'PutText' function */

    /* Put text function.
      * ARGUMENTS:
      *   - drawing string:
      *       const std::string &Str;
      *   - draw position:
      *       const ivec2 &Pos;
      *   - new text color:
      *       const DWORD &NewTextColor;
      * RETURNS: None.
      */
    VOID PutText( const std::string &Str, const ivec2 &Pos, const size &TextSize, const DWORD &NewTextColor )
    {
      TextColor = NewTextColor;

      RECT Rect = {Pos.X, Pos.Y, Pos.X + TextSize.W, Pos.Y + TextSize.H};

      DrawText(hMemDC, Str.c_str(), -1, &Rect, TextFormat);
    } /* End of 'PutText' function */

    /* Put text function.
     * ARGUMENTS:
     *   - drawing string:
     *       const std::string &Str;
     *   - draw position:
     *       const ivec2 &Pos;
     *   - new text color:
     *       const DWORD &NewTextColor;
     *   - other text format:
     *       const INT &OtherTextFormat;
     * RETURNS: None.
     */
    VOID PutText( const std::string &Str, const ivec2 &Pos, const size &TextSize, const DWORD &NewTextColor, const INT &OtherTextFormat )
    {
      TextColor = NewTextColor;
      
      RECT Rect = {Pos.X, Pos.Y, Pos.X + TextSize.W, Pos.Y + TextSize.H};
    
      DrawText(hMemDC, Str.c_str(), -1, &Rect, OtherTextFormat);
    } /* End of 'PutText' function */

    /* Put text in frame function.
     * ARGUMENTS:
     *   - drawing string:
     *       const std::string &Str;
     *   - textx position:
     *       const ivec2 &Pos;
     *   - text size:
     *       const size &TextSize;
     *   - text offset:
     *       const INT &Offset;
     * RETURNS: None.
     */
    VOID PutTextIn( const std::string &Str, const ivec2 &Pos, const size &TextSize, const INT &Offset )
    {
      RECT Rect = {Pos.X + Offset, Pos.Y, Pos.X + TextSize.W - Offset, Pos.Y + TextSize.H};

      DrawText(hMemDC, Str.c_str(), -1, &Rect, TextFormat);
    } /* End of 'PutTextIn' function */

    /* Copy frame function.
      * ARGUMENTS:
      *   - window handle:
      *       const HWND &hWnd;
      * RETURNS: None.
      */
    VOID CopyFrame( const HWND &hWnd )
    {
      RECT ClientRect;
        
      GetClientRect(hWnd, &ClientRect);

      BitBlt(hDC,
        0, 0,
        ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top,
        hMemDC,
        0, 0,
        SRCCOPY);
    } /* End of 'CopyFrame' function */
  }; /* End of 'font' class */
} /* end of 'tmp' class */

#endif // __font_def_h_

/* END OF 'font_def.h' FILE */