#ifndef __2d_renderer_h_
#define __2d_renderer_h_

#include "ui_def.h"

namespace ui
{
  /* Mask class */
  class mask
  {
  private:

    ivec2 Pos0, Pos1;
    
  public:

    /* Mask construct function */
    mask( VOID ) :
      Pos0({0, 0}),
      Pos1({0, 0})
    {
    } /* End of 'mask' function */

    /* Mask construct function */
    mask( const ivec2 &NewPos0, const ivec2 &NewPos1 ) :
      Pos0(NewPos0),
      Pos1(NewPos1)
    {
    } /* End of 'mask' function */

    /* Mask construct function */
    mask( const ivec2 &NewPos0, const isize2 &NewSize ) :
      Pos0(NewPos0),
      Pos1({Pos0.X + NewSize.W, Pos0.Y + NewSize.H})
    {
    } /* End of 'mask' function */

    /* Intersect point with mask function */
    BOOL operator()( const ivec2 &Point ) const
    {
      return
        Point.X >= Pos0.X && Point.X <= Pos1.X &&
        Point.Y >= Pos0.Y && Point.Y <= Pos1.Y;
    } /* End of 'operator()' function */

    /* Intersect line with mask function */
    BOOL operator()( UINT Y, INT &X0, INT X1 ) const
    {
      if (Y < Pos0.Y || Y > Pos1.Y)
        return false;

      X0 = std::max(X0, Pos0.X);
      X1 = std::min(X1, Pos1.X);
      return X0 <= X1;
    } /* End of 'operator()' function */

    BOOL IsExist( VOID ) const
    {
      return Pos1.Y >= Pos0.Y && Pos1.X >= Pos0.X;
    } /* End of 'isExist' function */

    mask Intersect( const mask &M ) const
    {
      return mask(ivec2(std::max(Pos0.X, M.Pos0.X), std::max(Pos0.Y, M.Pos0.Y)),
                  ivec2(std::min(Pos1.X, M.Pos1.X), std::min(Pos1.Y, M.Pos1.Y)));
    } /* End of 'Intersect' function */

  }; /* End of 'mask' class */

  /* 2d renderer class */
  class render_2d
  {
  public:

    HWND hWnd;            // Window handle
    std::vector<DWORD> Frame;      // Frame bits array
    isize2 FrameSize;     // Frame size
    INT FrameSizeMul;     // W mul H (for fast check)
    mask FrameMask;       // Frame mask


    render_2d( const isize2 &NewFrameSize ) :
      FrameSize(NewFrameSize),
      FrameSizeMul(NewFrameSize.W * NewFrameSize.H),
      Frame(NewFrameSize.W * NewFrameSize.H),
      FrameMask({0, 0}, FrameSize)
    {
    } /* End of 'render_2d' function */

    ~render_2d( VOID )
    {
    } /* End of 'render_2d' function */

    VOID Resize( const isize2 &NewSize )
    {
      Frame.clear();
      FrameSize = NewSize;
      FrameSizeMul = FrameSize.W * FrameSize.H;
      Frame = std::vector<DWORD>(FrameSize.W * FrameSize.H);
      FrameMask = mask({0, 0}, FrameSize);

      ClearFrame();
    } /* End of 'Resize' function */

    VOID SetFrame( DWORD Color )
    {
      std::fill(Frame.begin(), Frame.end(), Color);
      //memset(Frame, Color, FrameSize.W * FrameSize.H * sizeof(DWORD));
    } /* End of 'SetFrame' function */

    VOID ClearFrame( VOID )
    {
      std::fill(Frame.begin(), Frame.end(), 0);

      //memset(Frame, 0, FrameSize.W * FrameSize.H * sizeof(DWORD));
    } /* End of 'SetFrame' function */

    /* Put pixel functions */

    inline VOID PutPixelUnsafe( UINT Addr, DWORD Color )
    {
      Frame[Addr] = Color;
    } /* End of 'PutPixelUnsafe' function */

    inline VOID PutPixelUnsafe( const ivec2 &Pos, DWORD Color )
    {
      Frame[Pos.Y * FrameSize.W + Pos.X] = Color;
    } /* End of 'PutPixelUnsafe' function */

    VOID PutPixel( UINT Addr, DWORD Color )
    {
      if (Addr < FrameSizeMul)
        Frame[Addr] = Color;
    } /* End of 'PutPixel' function */

    VOID PutPixel( const ivec2 &Pos, DWORD Color )
    {
      if (Pos.X > 0 && Pos.X < FrameSize.W &&
          Pos.Y > 0 && Pos.Y < FrameSize.H)
        Frame[Pos.Y * FrameSize.W + Pos.X] = Color;
    } /* End of 'PutPixel' function */

    VOID PutPixel( const ivec2 &Pos, DWORD Color, const mask &Mask )
    {
      if (Mask(Pos))
        PutPixel(Pos, Color);
    } /* End of 'PutPixel' function */

    /* Put only bar function (without inner space) */
    VOID PutBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD FrameColor, mask Mask )
    {
      Mask = Mask.Intersect(FrameMask);

      if (!Mask.IsExist())
        return;

      INT X0, X1;

      /* Horisontal lines */

      // Top
      X0 = Pos0.X;
      X1 = Pos1.X;
      if (Mask(Pos0.Y, X0, X1))
        std::fill(Frame.begin() + Pos0.Y * FrameSize.W + X0, Frame.begin() + Pos0.Y * FrameSize.W + X1 + 1, FrameColor);
      // Bottom
      X0 = Pos0.X;
      X1 = Pos1.X;
      if (Mask(Pos1.Y, X0, X1))
        std::fill(Frame.begin() + Pos1.Y * FrameSize.W + X0, Frame.begin() + Pos1.Y * FrameSize.W + X1 + 1, FrameColor);


      /* Vertical lines */
      INT X0m, X1m;
      for (INT y = Pos0.Y; y < Pos1.Y; y++)
      {
        X0m = Pos0.X;
        X1m = Pos1.X;

        if (Mask(y, X0m, X1m))
        {
          PutPixelUnsafe(y * FrameSize.W + X0m, FrameColor);
          PutPixelUnsafe(y * FrameSize.W + X1m, FrameColor);
        }
      }
    } /* End of 'PutBar' function */

    /* Put filled bar function */
    VOID PutBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD FrameColor, DWORD SpaceColor, mask Mask )
    {
      Mask = Mask.Intersect(FrameMask);

      if (!Mask.IsExist())
        return;

      INT X0, X1;

      /* Horisontal lines */

      // Top
      X0 = Pos0.X;
      X1 = Pos1.X;
      if (Mask(Pos0.Y, X0, X1))
        std::fill(Frame.begin() + Pos0.Y * FrameSize.W + X0, Frame.begin() + Pos0.Y * FrameSize.W + X1 + 1, FrameColor);

      // Bottom
      X0 = Pos0.X;
      X1 = Pos1.X;
      if (Mask(Pos1.Y, X0, X1))
        std::fill(Frame.begin() + Pos1.Y * FrameSize.W + X0, Frame.begin() + Pos1.Y * FrameSize.W + X1 + 1, FrameColor);


      /* Vertical lines */
      INT X0m, X1m;
      for (INT y = Pos0.Y + 1; y < Pos1.Y; y++)
      {
        X0m = Pos0.X;
        X1m = Pos1.X;

        if (Mask(y, X0m, X1m))
        {
          std::fill(Frame.begin() + y * FrameSize.W + X0m, Frame.begin() + y * FrameSize.W + X1m + 1, SpaceColor);
          PutPixelUnsafe(y * FrameSize.W + X0m, FrameColor);
          PutPixelUnsafe(y * FrameSize.W + X1m, FrameColor);
        }
      }
    } /* End of 'PutBar' function */

    /* The same functions with size argument */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, mask Mask )
    {
      PutBar(Pos0, ivec2(Pos0.X + Size.W, Pos0.Y + Size.H), FrameColor, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, DWORD SpaceColor, mask Mask )
    {
      PutBar(Pos0, ivec2(Pos0.X + Size.W, Pos0.Y + Size.H), FrameColor, SpaceColor, Mask);
    } /* End of 'PutBar' function */
    
    
      
    // VOID CopyFrame( const HWND &hWnd )
    // {
    //   RECT ClientRect;
    //     
    //   GetClientRect(hWnd, &ClientRect);
    // 
    //   BitBlt(hDC,
    //     0, 0,
    //     ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top,
    //     hMemDC,
    //     0, 0,
    //     SRCCOPY);
    // } /* End of 'CopyFrame' function */
  }; /* End of 'render_2d' class */
   
} /* end of 'ui' namespace */

#endif // __2d_renderer_h_