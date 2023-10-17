/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : gfx_def.cpp
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

#include "gfx_def.h"

namespace tmp
{
  
    // gfx class functions

    /* Set frame function.
     * ARGUMENTS:
     *   - color:
     *       const DWORD &Color;
     * RETURNS: None.
     */
    VOID gfx::SetFrame( const DWORD &Color )
    {
      //for (INT i = 0; i < FrameSizeMul; i++)
      //  Frame[i] = Color;

      memset(Frame, Color, FrameSize.W * FrameSize.H * sizeof(DWORD));
    } /* End of 'gfx::SetFrame' function */

    /* Clear frame - set black color function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID gfx::ClearFrame( VOID )
    {
      memset(Frame, 0, FrameSize.W * FrameSize.H * sizeof(DWORD));
    } /* End of 'gfx::SetFrame' function */

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
    VOID gfx::PutPixel( const ivec2 &Pos, const DWORD &Color, const INT &PointSize )
    {
      if (Pos.X < FrameSize.W && Pos.Y < FrameSize.H)
        Frame[Pos.Y * FrameSize.W + Pos.X] = Color;
    } /* End of 'gfx::PutPixel' function */

    /* Put pixel function.
     * ARGUMENTS:
     *   - pixel pos:
     *       const ITN &Addr;
     *   - color:
     *       const DWORD &Color;
     *   - point size:
     *       const INT &PointSize;
     * RETURNS: None.
     */
    VOID gfx::PutPixel( const INT &Addr, const DWORD &Color, const INT &PointSize )
    {
      switch (PointSize)
      {
      case 1:
        if (Addr < FrameSizeMul)
          Frame[Addr] = Color;
        return;
      default:
        for (INT y = -PointSize / 2.0; y < PointSize / 2.0; y++)
          for (INT x = -PointSize / 2.0; x < PointSize / 2.0; x++)
            if (Addr < FrameSizeMul)
              Frame[Addr + y * FrameSize.W + x] = Color;
        return;
      }
      
    } /* End of 'gfx::PutPixel' function */

    /* Line draw function.
     * ARGUMENTS:
     *   - first and lastest pos:
     *       ivec2 Pos0, Pos1;
     *   - input color:
     *       DWORD Color;
     * RETURNS: None.
     */
    VOID gfx::PutLine( const ivec2 &Pos0, const ivec2 &Pos1, const DWORD &Color )
    {
      INT
        X0 = Pos0.X, Y0 = Pos0.Y,
        X1 = Pos1.X, Y1 = Pos1.Y,
        dx = X1 - X0,
        dy = Y1 - Y0,
        IncrE, IncrNE,
        Count,
        x = X0, y = Y0,
        tmp, sx, sy, F;
  
      PutPixel(Pos0, Color);
  
      if (abs(dx) >= abs(dy))
      {
        if (dy < 0)
          sy = -1, dy = -dy;
        else
          sy = 1;
        if (dx < 0)
          sx = -1, dx = -dx;
        else
          sx = 1;
        F = 2 * dy - dx;
        IncrE = 2 * dy;
        IncrNE = 2 * dy - 2 * dx;
        Count = dx;
        while (Count-- > 0)
        {
          if (F > 0)
            y += sy, F += IncrNE;
          else
            F += IncrE;
          x += sx;
          PutPixel(ivec2(x, y), Color);
        }
      }
      else
      {
        tmp = X0, X0 = X1, X1 = tmp;
        tmp = Y0, Y0 = Y1, Y1 = tmp;
      
        if (dy < 0)
          sy = -1, dy = -dy;
        else
          sy = 1;
        if (dx < 0)
          sx = -1, dx = -dx;
        else
          sx = 1;
  
        F = 2 * dx - dy;
        IncrE = 2 * dx;
        IncrNE = 2 * dx - 2 * dy;
        Count = dy;
        while (Count-- > 0)
        {
          if (F > 0)
            x += sx, F += IncrNE;
          else
            F += IncrE;
          y += sy;
          PutPixel(ivec2(x, y), Color);
        }
      }
    } /* End of 'gfx::PutLine' function */

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
    VOID gfx::PutCircle( const ivec2 &Pos0, INT R, const DWORD &FrameColor, const DWORD &SpaceColor )
    {
      INT
        X0 = Pos0.X, Y0 = Pos0.Y,
        F, x, y;
    
      F = 1 - R;
      x = 0;
      y = R;

      
        
      if (SpaceColor != 0)
      {
        for (INT ix = -R; ix < R; ix++)
          PutPixel(ivec2(X0 + ix, Y0), SpaceColor);
  
        PutPixel(ivec2(X0, Y0 + y), SpaceColor);
        PutPixel(ivec2(X0, Y0 - y), SpaceColor);
        while (x <= y)
        {
          if (F >= 0)
            y--, F += 2 * x - 2 * y + 5;
          else
            F += 2 * x + 3;
          x++;
          
          for (INT ix = -x; ix < x; ix++)
            PutPixel(ivec2(X0 + ix, Y0 + y), SpaceColor);

          for (INT ix = -x; ix < x; ix++)
            PutPixel(ivec2(X0 + ix, Y0 - y), SpaceColor);

          for (INT ix = -y; ix < y; ix++)
            PutPixel(ivec2(X0 + ix, Y0 + x), SpaceColor);

          for (INT ix = -y; ix < y; ix++)
            PutPixel(ivec2(X0 + ix, Y0 - x), SpaceColor);
        }
      }
        
      F = 1 - R;
      x = 0;
      y = R;

      if (FrameColor != 0)
      {
        PutPixel(ivec2(X0 + R, Y0), FrameColor);
        PutPixel(ivec2(X0 - R, Y0), FrameColor);
        PutPixel(ivec2(X0, Y0 + y), FrameColor);
        PutPixel(ivec2(X0, Y0 - y), FrameColor);

        while (x <= y)
        {
          if (F >= 0)
            y--, F += 2 * x - 2 * y + 5;
          else
            F += 2 * x + 3;
          x++;
          
          PutPixel(ivec2(X0 + x, Y0 + y), FrameColor);
          PutPixel(ivec2(X0 - x, Y0 + y), FrameColor);

          PutPixel(ivec2(X0 + x, Y0 - y), FrameColor);
          PutPixel(ivec2(X0 - x, Y0 - y), FrameColor);

          PutPixel(ivec2(X0 + y, Y0 + x), FrameColor);
          PutPixel(ivec2(X0 - y, Y0 + x), FrameColor);

          PutPixel(ivec2(X0 + y, Y0 - x), FrameColor);
          PutPixel(ivec2(X0 - y, Y0 - x), FrameColor);
        }
      }
    } /* End of 'gfx::PutCircle' function */

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
    VOID gfx::PutBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &SpaceColor )
    {
      ivec2 Pos2;
      DWORD addr1, addr2;
    
      /* Trivial reject */
      if (BarSize.W <= 0 || BarSize.H <= 0 || Pos1.X >= (INT)FrameSize.W || Pos1.Y >= (INT)FrameSize.H)
        return;

      Pos2 = ivec2(Pos1.X + BarSize.W - 1, Pos1.Y + BarSize.H - 1);
      
      if (Pos2.X < 0 || Pos2.Y < 0)
        return;
    
      /* Clipping */
      if (Pos1.X < 0)
        Pos1.X = 0;
      if (Pos1.Y < 0)
        Pos1.Y = 0;
      if (Pos2.X >= (INT)FrameSize.W)
        Pos2.X = FrameSize.W - 1;
      if (Pos2.Y >= (INT)FrameSize.H)
        Pos2.Y = FrameSize.H - 1;
    
      BarSize.W = Pos2.X - Pos1.X + 1;
      BarSize.H = Pos2.Y - Pos1.Y + 1;
    
      addr1 = Pos1.Y * FrameSize.W + Pos1.X;
      addr2 = Pos2.Y * FrameSize.W + Pos1.X;
      
      if (SpaceColor != 0)
      {
        size TempBarSize = BarSize;
        INT addr = Pos1.Y * FrameSize.W + Pos1.X;
        INT delta = FrameSize.W - BarSize.W;

        while (TempBarSize.H-- > 0)
        {
          INT cnt = TempBarSize.W;
          while (cnt-- > 0)
            PutPixel(addr++, SpaceColor);
          addr += delta;
        }
      }

      if (FrameColor != 0)
      {
        size TempBarSize = BarSize;
        
        while (TempBarSize.W-- > 0)
        {
          PutPixel(addr1++, FrameColor);
          PutPixel(addr2++, FrameColor);
        }
    
        addr2 = Pos1.Y * FrameSize.W + Pos1.X;
        while (TempBarSize.H-- > 0)
        {
          PutPixel(addr1, FrameColor);
          PutPixel(addr2, FrameColor);
    
          addr1 += (INT)FrameSize.W;
          addr2 += (INT)FrameSize.W;
        }
      }
    } /* End of 'gfx::PutBar' function */

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
    VOID gfx::PutBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &SpaceColor, const INT R, const INT &RoundFlag )
    {
      INT PointSize = 1;
      ivec2
        OutPos1 = Pos1,
        OutPos2 = Pos1 + ivec2(BarSize.W, BarSize.H),
        InPos1 = OutPos1 + ivec2(R, R),
        InPos2 = OutPos2 - ivec2(R, R);
      BOOL
        FrameDrawFlag = FrameColor != 0,
        SpaceDrawFlag = SpaceColor != 0;

      if (!FrameDrawFlag && !SpaceDrawFlag)
        return;

      // !!! Not without clipping because every putpixel has check on bolders

      /****
       * Draw bar part 1 - space without round
       ****/
      
      /**** Draw horizontal space ****/

      INT
        YAddr = InPos1.Y * FrameSize.W + OutPos1.X;

      for (INT y = InPos1.Y; y <= InPos2.Y; y++, YAddr += FrameSize.W)
      {
        if (SpaceDrawFlag)
         for (INT XAddr = YAddr, x = OutPos1.X; x <= OutPos2.X; x++, XAddr++)
           PutPixel(XAddr, SpaceColor, PointSize);

        if (FrameDrawFlag)
        {
          PutPixel(YAddr, FrameColor, PointSize);
          PutPixel(YAddr + OutPos2.X - OutPos1.X, FrameColor, PointSize);
        }
      }

      /****
       * Draw bar part 2 - round spaces
       ****/

      if (SpaceDrawFlag)
      {
        INT F = 1 - R;
        ivec2
          CircleDelta = {0, R};
        size DeltaSize;

        while (CircleDelta.X <= CircleDelta.Y)
        {
          if (F >= 0)
            CircleDelta.Y--, F += 2 * CircleDelta.X - 2 * CircleDelta.Y + 5;
          else
            F += 2 * CircleDelta.X + 3;
          CircleDelta.X++;

          // Draw top top part
          INT
            XAddrBegin = (InPos1.Y - CircleDelta.Y) * FrameSize.W + InPos1.X,
            XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
            PutPixel(Addr, SpaceColor, PointSize);
          
          // Draw top bottom part
          XAddrBegin = (InPos1.Y - CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
            PutPixel(Addr, SpaceColor, PointSize);

          // Draw bottom bottom part

          XAddrBegin = (InPos2.Y + CircleDelta.Y) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
            PutPixel(Addr, SpaceColor, PointSize);

          // Draw bottom top part

          XAddrBegin = (InPos2.Y + CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
            PutPixel(Addr, SpaceColor, PointSize);
        }
      }
      // Draw only round frame
      if (FrameDrawFlag)
      {
        INT F = 1 - R;
        ivec2
          CircleDelta = {0, R};
        size DeltaSize;

        while (CircleDelta.X <= CircleDelta.Y)
        {
          if (F >= 0)
            CircleDelta.Y--, F += 2 * CircleDelta.X - 2 * CircleDelta.Y + 5;
          else
            F += 2 * CircleDelta.X + 3;
          CircleDelta.X++;

          // Draw top top part
          INT
            XAddrBegin = (InPos1.Y - CircleDelta.Y) * FrameSize.W + InPos1.X,
            XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);
          
          // Draw top bottom part
          XAddrBegin = (InPos1.Y - CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);

          // Draw bottom bottom part

          XAddrBegin = (InPos2.Y + CircleDelta.Y) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);

          // Draw bottom top part

          XAddrBegin = (InPos2.Y + CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);
        }
      }

      /****
       * Draw bar part 3 - Draw top and botto, frame line
       ****/

       /**** Draw top frame line ****/

      INT
        LineBeginAddr = OutPos1.Y * FrameSize.W + OutPos1.X,
        LineEndAddr = OutPos1.Y * FrameSize.W + OutPos2.X;

      // Round vertexes
      if (RoundFlag & RV_LEFT_TOP)
        LineBeginAddr += R;
      if (RoundFlag & RV_RIGHT_TOP)
        LineEndAddr -= R;

      if (FrameDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, FrameColor, PointSize);
      else if (SpaceDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, SpaceColor, PointSize);

      // Draw bottom line
      LineBeginAddr = OutPos2.Y * FrameSize.W + OutPos1.X;
      LineEndAddr = OutPos2.Y * FrameSize.W + OutPos2.X;

      // Round vertexes
      if (RoundFlag & RV_LEFT_BOTTOM)
        LineBeginAddr += R;
      if (RoundFlag & RV_RIGHT_BOTTOM)
        LineEndAddr -= R;

      if (FrameDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, FrameColor, PointSize);
      else if (SpaceDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, SpaceColor, PointSize);


    } /* End of 'gfx::PutRoundBar' function */

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
    VOID gfx::PutSeparatedBar( ivec2 Pos1, size BarSize, const DWORD &FrameColor, const DWORD &Space1Color, const DWORD &Space2Color, const ivec2 &InSepPos, const INT R, const INT &RoundFlag )
    {
      INT PointSize = 1;
      ivec2
        OutPos1 = Pos1,
        OutPos2 = Pos1 + ivec2(BarSize.W, BarSize.H),
        InPos1 = OutPos1 + ivec2(R, R),
        InPos2 = OutPos2 - ivec2(R, R);
      BOOL
        FrameDrawFlag = FrameColor != 0,
        Space1DrawFlag = Space1Color != 0,
        Space2DrawFlag = Space2Color != 0,
        SepOnX = 0,
        SepOnY = 0;

      if (!FrameDrawFlag && !Space1DrawFlag && !Space2DrawFlag)
        return;

      if (InSepPos.Y != 0)
        SepOnY = 1;
      else
        SepOnX = 1;

      ivec2 SepPos = Pos1 + InSepPos;

      // !!! Not without clipping because every putpixel has check on bolders

      /****
       * Draw bar part 1 - space without round
       ****/
      
      /**** Draw horizontal space ****/

      INT
        YAddr = InPos1.Y * FrameSize.W + OutPos1.X;

      if (SepOnY)
      {
        for (INT y = InPos1.Y; y <= InPos2.Y; y++, YAddr += FrameSize.W)
        {
          if (y < SepPos.Y)
          {
            if (Space1DrawFlag)
              for (INT XAddr = YAddr, x = OutPos1.X; x <= OutPos2.X; x++, XAddr++)
                PutPixel(XAddr, Space1Color, PointSize);
          }
          else
            if (Space2DrawFlag)
              for (INT XAddr = YAddr, x = OutPos1.X; x <= OutPos2.X; x++, XAddr++)
                PutPixel(XAddr, Space2Color, PointSize);

          if (FrameDrawFlag)
          {
            PutPixel(YAddr, FrameColor, PointSize);
            PutPixel(YAddr + OutPos2.X - OutPos1.X, FrameColor, PointSize);
          }
        }
      }
      else
      {
        for (INT y = InPos1.Y; y <= InPos2.Y; y++, YAddr += FrameSize.W)
        {
          for (INT XAddr = YAddr, x = OutPos1.X; x <= OutPos2.X; x++, XAddr++)
            if (x < SepPos.X)
            {
              if (Space1DrawFlag)
                PutPixel(XAddr, Space1Color, PointSize);
            }
            else
              if (Space2DrawFlag)
                PutPixel(XAddr, Space2Color, PointSize);

          if (FrameDrawFlag)
          {
            PutPixel(YAddr, FrameColor, PointSize);
            PutPixel(YAddr + OutPos2.X - OutPos1.X, FrameColor, PointSize);
          }
        }
      }

      
      

      /****
       * Draw bar part 2 - round spaces
       ****/

      if (Space1DrawFlag || Space2DrawFlag)
      {
        INT F = 1 - R;
        ivec2
          CircleDelta = {0, R};
        size DeltaSize;

        while (CircleDelta.X <= CircleDelta.Y)
        {
          if (F >= 0)
            CircleDelta.Y--, F += 2 * CircleDelta.X - 2 * CircleDelta.Y + 5;
          else
            F += 2 * CircleDelta.X + 3;
          CircleDelta.X++;

          // Draw top top part
          INT
            y = InPos1.Y - CircleDelta.Y,
            XAddrBegin = y * FrameSize.W + InPos1.X,
            XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          if (SepOnY)
          {
            if (y < SepPos.Y)
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space1Color, PointSize);
            else
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space2Color, PointSize);
          }
          else
            for (INT Addr = XAddrBegin, x = XAddrBegin - y * FrameSize.W; Addr <= XAddrEnd; Addr++, x++)
              if (x < SepPos.X)
                PutPixel(Addr, Space1Color, PointSize);
              else
                PutPixel(Addr, Space2Color, PointSize);
          
          // Draw top bottom part
          y = InPos1.Y - CircleDelta.X;
          XAddrBegin = y * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          if (SepOnY)
          {
            if (y < SepPos.Y)
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space1Color, PointSize);
            else
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space2Color, PointSize);
          }
          else
            for (INT Addr = XAddrBegin, x = XAddrBegin - y * FrameSize.W; Addr <= XAddrEnd; Addr++, x++)
              if (x < SepPos.X)
                PutPixel(Addr, Space1Color, PointSize);
              else
                PutPixel(Addr, Space2Color, PointSize);

          // Draw bottom bottom part

          y = InPos2.Y + CircleDelta.Y;
          XAddrBegin = y * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          if (SepOnY)
          {
            if (y < SepPos.Y)
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space1Color, PointSize);
            else
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space2Color, PointSize);
          }
          else
            for (INT Addr = XAddrBegin, x = XAddrBegin - y * FrameSize.W; Addr <= XAddrEnd; Addr++, x++)
              if (x < SepPos.X)
                PutPixel(Addr, Space1Color, PointSize);
              else
                PutPixel(Addr, Space2Color, PointSize);

          // Draw bottom top part

          y = InPos2.Y + CircleDelta.X;
          XAddrBegin = y * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          if (SepOnY)
          {
            if (y < SepPos.Y)
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space1Color, PointSize);
            else
              for (INT Addr = XAddrBegin; Addr <= XAddrEnd; Addr++)
                PutPixel(Addr, Space2Color, PointSize);
          }
          else
            for (INT Addr = XAddrBegin, x = XAddrBegin - y * FrameSize.W; Addr <= XAddrEnd; Addr++, x++)
              if (x < SepPos.X)
                PutPixel(Addr, Space1Color, PointSize);
              else
                PutPixel(Addr, Space2Color, PointSize);
        }
      }

      // Draw only round frame
      if (FrameDrawFlag)
      {
        INT F = 1 - R;
        ivec2
          CircleDelta = {0, R};
        size DeltaSize;

        while (CircleDelta.X <= CircleDelta.Y)
        {
          if (F >= 0)
            CircleDelta.Y--, F += 2 * CircleDelta.X - 2 * CircleDelta.Y + 5;
          else
            F += 2 * CircleDelta.X + 3;
          CircleDelta.X++;

          // Draw top top part
          INT
            XAddrBegin = (InPos1.Y - CircleDelta.Y) * FrameSize.W + InPos1.X,
            XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);
          
          // Draw top bottom part
          XAddrBegin = (InPos1.Y - CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_TOP)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_TOP)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);

          // Draw bottom bottom part

          XAddrBegin = (InPos2.Y + CircleDelta.Y) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.X;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.X;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);

          // Draw bottom top part

          XAddrBegin = (InPos2.Y + CircleDelta.X) * FrameSize.W + InPos1.X;
          XAddrEnd = XAddrBegin + InPos2.X - InPos1.X;

          if (RoundFlag & RV_LEFT_BOTTOM)
            XAddrBegin -= CircleDelta.Y;
          else
            XAddrBegin -= R;

          if (RoundFlag & RV_RIGHT_BOTTOM)
            XAddrEnd += CircleDelta.Y;
          else
            XAddrEnd += R;

          PutPixel(XAddrBegin, FrameColor, PointSize);
          PutPixel(XAddrEnd, FrameColor, PointSize);
        }
      }

      /****
       * Draw bar part 3 - Draw top and botto, frame line
       ****/

       /**** Draw top frame line ****/

      INT
        LineBeginAddr = OutPos1.Y * FrameSize.W + OutPos1.X,
        LineEndAddr = OutPos1.Y * FrameSize.W + OutPos2.X;

      // Round vertexes
      if (RoundFlag & RV_LEFT_TOP)
        LineBeginAddr += R;
      if (RoundFlag & RV_RIGHT_TOP)
        LineEndAddr -= R;

      if (FrameDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, FrameColor, PointSize);
      else if (Space1DrawFlag)
        if (SepOnY)
        {
          if (OutPos1.Y < SepPos.Y)
            for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
              PutPixel(Addr, Space1Color, PointSize);
          else
            for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
              PutPixel(Addr, Space2Color, PointSize);
        }
        else
          for (INT Addr = LineBeginAddr, x = OutPos1.X; Addr <= LineEndAddr; Addr++, x++)
            if (x < SepPos.X)
              PutPixel(Addr, Space1Color, PointSize);
            else
              PutPixel(Addr, Space2Color, PointSize);


      // Draw bottom line
      LineBeginAddr = OutPos2.Y * FrameSize.W + OutPos1.X;
      LineEndAddr = OutPos2.Y * FrameSize.W + OutPos2.X;

      // Round vertexes
      if (RoundFlag & RV_LEFT_BOTTOM)
        LineBeginAddr += R;
      if (RoundFlag & RV_RIGHT_BOTTOM)
        LineEndAddr -= R;

      if (FrameDrawFlag)
        for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
          PutPixel(Addr, FrameColor, PointSize);
      else if (Space1DrawFlag)
        if (SepOnY)
        {
          if (OutPos2.Y < SepPos.Y)
            for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
              PutPixel(Addr, Space1Color, PointSize);
          else
            for (INT Addr = LineBeginAddr; Addr <= LineEndAddr; Addr++)
              PutPixel(Addr, Space2Color, PointSize);
        }
        else
          for (INT Addr = LineBeginAddr, x = OutPos1.X; Addr <= LineEndAddr; Addr++, x++)
            if (x < SepPos.X)
              PutPixel(Addr, Space1Color, PointSize);
            else
              PutPixel(Addr, Space2Color, PointSize);
    } /* End of 'gfx::PutRoundBar' function */

} /* end of 'tmp' namespace */

/* END OF 'gfx_def.cpp' FILE */