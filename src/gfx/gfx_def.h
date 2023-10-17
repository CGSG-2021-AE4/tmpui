/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : gfx_def.h
  * PURPOSE     : Skinning project.
  *               Main 2d grafics handle module.
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

#ifndef __gfx_def_h_
#define __gfx_def_h_

#include "font_def.h"

//#include <tmp.h>

namespace tmp
{
    class gfx : public font
    {
    public:

      HWND hWnd;       // Window handle
      DWORD *Frame {}; // Frame bits array
      size FrameSize;  // Frame size
      INT FrameSizeMul;     // W mul H (for fast check)

      /* 'gfx' default construct function.
       * ARGUMENTS:
       *   - window handle:
       *       const HWND &NewhWnd;
       *   - new w, h:
       *       const UINT &NewW, &NewH;
       *   - text format:
       *       const BYTE &TextFormat;
       * RETURNs: None.
       */
      gfx( const HWND &NewhWnd, const size &NewFrameSize, const BYTE &TextFormat ) :
        FrameSize(NewFrameSize), FrameSizeMul(NewFrameSize.W * NewFrameSize.H), Frame(new DWORD[NewFrameSize.W * NewFrameSize.H]), hWnd(NewhWnd),
        font(NewhWnd, 15, TextFormat, RGB(255, 255, 255))
      {
      } /* End of 'gfx' function */

      /* 'gfx' default destruct function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      ~gfx( VOID )
      {
        delete[FrameSizeMul] Frame;
      } /* End of 'gfx' function */

      /* Gfx resize function.
       * ARUGMENTS:
       *   - new size:
       *       const size &NewSize;
       * RETURNS: None.
       */
      VOID Resize( const size &NewSize )
      {
        delete[FrameSize.W * FrameSize.H] Frame;

        FrameSize = NewSize;

        FrameSizeMul = FrameSize.W * FrameSize.H;

        Frame = new DWORD[FrameSize.W * FrameSize.H];

        ClearFrame();
      } /* End of 'Resize' function */

      /* Set frame function.
       * ARGUMENTS:
       *   - color:
       *       const DWORD &Color;
       * RETURNS: None.
       */
      VOID SetFrame( const DWORD &Color );

      /* Clear frame - set black color function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ClearFrame( VOID );

      /* Put pixel function.
       * ARGUMENTS:
       *   - pixel pos:
       *       const ivec2 &Pos;
       *   - color:
       *       const DWORD &Color;
       *   - point size:
       *       const INT &PointSize;
       * RETURNS: None.
       */
      VOID PutPixel( const ivec2 &Pos, const DWORD &Color, const INT &PointSize = 1 );

      /* Put pixel function.
       * ARGUMENTS:
       *   - pixel addr:
       *       const INT &Addr;
       *   - color:
       *       const DWORD &Color;
       *   - point size:
       *       const INT &PointSize;
       * RETURNS: None.
       */
      VOID PutPixel( const INT &Addr, const DWORD &Color, const INT &PointSize = 1 );
    
      /* Line draw function.
       * ARGUMENTS:
       *   - first and lastest pos:
       *       ivec2 Pos0, Pos1;
       *   - input color:
       *       DWORD Color;
       * RETURNS: None.
       */
      VOID PutLine( const ivec2 &Pos0, const ivec2 &Pos1, const DWORD &Color );
      
      /* Circle draw function.
       * ARGUMENTS:
       *   - center pos:
       *       const ivec2 &Pos0;
       *   - circle radius:
       *       INT R;
       *   - input frame and space color:
       *       const DWORD &OutColor;
       * RETURNS: None.
       */
      VOID PutCircle( const ivec2 &Pos0, INT R, const DWORD &FrameColor, const DWORD &SpaceColor );

      /* Put bar function.
       * ARGUMENTS:
       *   - frame position:
       *       ivec2 Pos1;
       *   - bar size:
       *       size BarSize;
       *   - bar color:
       *       DWORD Color;
       * RETURNS: None.
       */
      VOID PutBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &SpaceColor );

      typedef enum
      {
        RV_LEFT_TOP     = 1,
        RV_RIGHT_TOP    = 2,
        RV_LEFT_BOTTOM  = 4,
        RV_RIGHT_BOTTOM = 8,
        RV_ALL = 15,
      } ROUND_VERTS;

      /* Put rounded bar function.
       * ARGUMENTS:
       *   - frame position:
       *       ivec2 Pos1;
       *   - bar size:
       *       size BarSize;
       *   - bar color:
       *       DWORD Color;
       *   - rounded part radious:
       *       const INT R;
       *   - which vertexes are rounded enum:
       *       const ROUND_VERTS &RoundFlag;
       * RETURNS: None.
       */
      VOID PutBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &SpaceColor, const INT R, const INT &RoundFlag );

      /* Put separated rounded bar function.
       * ARGUMENTS:
       *   - frame position:
       *       ivec2 Pos1;
       *   - bar size:
       *       size BarSize;
       *   - bar frame color:
       *       const DWORD &FrameColor;
       *   - spaces color:
       *       const DWORD &Space1Color, &Space2Color;
       *   - separate pos:
       *       const ivec2 &InSepPos;
       *   - rounded part radious:
       *       const INT R;
       *   - which vertexes are rounded enum:
       *       const ROUND_VERTS &RoundFlag;
       * RETURNS: None.
       */
      VOID PutSeparatedBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &Space1Color, const DWORD &Space2Color, const ivec2 &InSepPos, const INT R, const INT &RoundFlag );
      
      /* Render frame on screen function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID RenderFrame( VOID )
      {
        CopyFrame(hWnd);
      } /* End of 'RenderFrame' function */
    }; /* End of 'gfx' class */
} /* end of 'tmp' class */

#endif // __gfx_def_h_

/* END OF 'gfx_def.h' FILE */