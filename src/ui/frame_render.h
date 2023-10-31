#include "ui_def.h"

#ifndef __2d_renderer_h_
#define __2d_renderer_h_

namespace ui
{
  /* 2d renderer class */
  class render_2d
  {
  private:

  public:

    isize2 FrameSize;     // Frame size
    INT FrameSizeMul;     // W mul H (for fast check)
    std::vector<DWORD> Frame;      // Frame bits array
    mask FrameMask;       // Frame mask
    isize2 CharSize;
    std::vector<std::vector<FLT>> CharSet;
    /* Char set setup function */
    VOID SetupCharSet( VOID )
    {
      // hDC, hMemDC
      HDC hDC {};
      HDC hMemDC {};
      
      hDC = GetDC(nullptr);
      hMemDC = CreateCompatibleDC(hDC);

      // Font
      INT FontH = 16;
      DWORD TextColor = 0xFFFFFFFF;
      
      CharSize = {FontH / 2, FontH};
      
      HFONT hFont = CreateFont(CharSize.H, CharSize.W, 0, 0, 0, 0, 0, 0,
                               ANSI_CHARSET,
                               OUT_DEVICE_PRECIS,
                               CLIP_DEFAULT_PRECIS,
                               DEFAULT_QUALITY,
                               DEFAULT_PITCH,
                               TEXT("Consolas"));

      // Apply font
      SelectObject(hMemDC, hFont);
      SetTextColor(hMemDC, TextColor);

      // Char bits, rect
      std::vector<DWORD> CharBits = std::vector<DWORD>(CharSize.W * CharSize.H);
      RECT CharRect = {0, 0, CharSize.W, CharSize.H};

      // Setup DIB section
      BITMAPINFO BMInfo {};
      DWORD *BitsPtr = nullptr;
      
      BMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      BMInfo.bmiHeader.biWidth = CharSize.W;
      BMInfo.bmiHeader.biHeight = CharSize.H;
      BMInfo.bmiHeader.biPlanes = 1;
      BMInfo.bmiHeader.biBitCount = 32;
      BMInfo.bmiHeader.biCompression = BI_RGB;
      HBITMAP hCharBm = CreateDIBSection(hMemDC, &BMInfo, DIB_RGB_COLORS, (void**)&BitsPtr, NULL, NULL);
      SelectObject(hMemDC, hCharBm);

      // Char draw
      for (INT i = 0; i < 256; i++)
      {
        CHAR c = i;
        TextOutA(hMemDC, 0, 0, &c, 1);
        GetBitmapBits(hCharBm, (INT)CharBits.size() * 4, CharBits.data());

        std::vector<FLT> CharMask(CharBits.size());

        for (UINT i = 1; i < (UINT)CharBits.size(); i++)
          CharMask[i] = ((0x00FFFFFF - CharBits[i]) & 0x000000FF) / 256.0;
        CharSet.push_back(CharMask);
      }
    } /* End pf 'SetupCharSet' function */

    /* Construct function */
    render_2d( const isize2 &NewFrameSize ) :
      FrameSize(NewFrameSize),
      FrameSizeMul(NewFrameSize.W * NewFrameSize.H),
      Frame(NewFrameSize.W * NewFrameSize.H),
      FrameMask({0, 0}, FrameSize)
    {
      SetupCharSet();
    } /* End of 'render_2d' function */

    /* Desructor function */
    ~render_2d( VOID )
    {
    } /* End of 'render_2d' function */

    // Text functions

    inline VOID PutChar( UCHAR c, const ivec2 &Pos, const vec3 &Color, const mask &Mask )
    {
      for (INT y = 0; y < CharSize.H; y++)
        for (INT x = 0; x < CharSize.W; x++) // Later this part will be on assembler
        {
          ivec2 PPos = Pos + ivec2(x, y);
          if (Mask(PPos))
          {
            INT Addr = PPos.Y * FrameSize.W + PPos.X;
            FLT Value = CharSet[c][y * CharSize.W + x];

            vec3 FColor = {GetRValue(Frame[Addr]) / 256.0,
                           GetGValue(Frame[Addr]) / 256.0,
                           GetBValue(Frame[Addr]) / 256.0};

            Frame[Addr] = ToRGB(Color * Value + FColor * (1 - Value));
            //Frame[Addr] = Value > 0 ? 0x00FFFFFF : 0;
          } 
            //PutPixel(Pos + ivec2(x, y), CharSet[c][y * CharSize.W + x], Mask);
        }
          //if (CharSet[c][y * CharSize.W + x])
    } /* End of 'PutChar' function */

    inline VOID PutStr( const std::string &Str, const ivec2 &Pos, const vec3 &Color, const mask &Mask )
    {
      for (UINT i = 0; i < (UINT)Str.size(); i++)
        PutChar(Str[i], Pos + ivec2(i * CharSize.W, 0), Color, Mask);
    } /* End of 'PutStr' function */

    enum struct vert_align
    {
      eTop    = 0x00000000,
      eCenter = 0x00000001,
      eBottom = 0x00000002,
    }; /* End of 'vert_align' enum struct */

    enum struct hor_align
    {
      eLeft   = 0x00000000,
      eCenter = 0x00000100,
      eRight  = 0x00000200,
    }; /* End of 'hor_align' enum struct */

    inline VOID PutStr( const std::string &Str, const ivec2 &Pos, const isize2 Size, const ivec2 &Padding, DWORD LayoutFlags, const vec3 &Color, const mask &Mask )
    {
      ivec2 OutPos = Pos + Padding; // Vertical align - top, Horizontal align - left

      // Check vertical align flags
      if (LayoutFlags & (DWORD)vert_align::eCenter)
        OutPos.Y = (INT)(Pos.Y + (Size.H - CharSize.H) / 2.0);
      if (LayoutFlags & (DWORD)vert_align::eBottom)
        OutPos.Y = Pos.Y + Size.H - Padding.Y - CharSize.H;

      // Check horizontal align flags
      if (LayoutFlags & (DWORD)hor_align::eCenter)
        OutPos.X = (INT)(Pos.X + (Size.W - CharSize.W * (INT)Str.size()) / 2.0);
      if (LayoutFlags & (DWORD)hor_align::eRight)
        OutPos.X = Pos.X + Size.W - Padding.X - CharSize.W * (INT)Str.size();

      PutStr(Str, OutPos, Color, Mask);
    } /* End of 'PutStr' function */

    /* Resize function */
    VOID Resize( const isize2 &NewSize )
    {
      Frame.clear();
      FrameSize = NewSize;
      FrameSizeMul = FrameSize.W * FrameSize.H;
      Frame = std::vector<DWORD>((size_t)(FrameSize.W * FrameSize.H));
      FrameMask = mask({0, 0}, FrameSize);

      ClearFrame();
    } /* End of 'Resize' function */

    /* Fill frame with color function */
    VOID SetFrame( DWORD Color )
    {
      std::fill(Frame.begin(), Frame.end(), Color);
    } /* End of 'SetFrame' function */

    /* Clear frame function */
    VOID ClearFrame( VOID )
    {
      std::fill(Frame.begin(), Frame.end(), 0);
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

    VOID PutPixel( INT Addr, DWORD Color )
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

    /* Put filled bar function */
    template<BOOL IsFilled = 1, BOOL IsBorderW1 = 0>
      VOID PutBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD FrameColor, DWORD SpaceColor, INT BorderW,  mask Mask )
      {
        Mask = Mask.Intersect(FrameMask);

        if (!Mask.IsExist())
          return;

        INT X0, X1;

        /* Horisontal lines */

        // Top
        if constexpr (IsBorderW1)
        {
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
        }
        else
        {
          for (INT Offset = 0; Offset < BorderW; Offset++)
          {
            X0 = Pos0.X;
            X1 = Pos1.X;

            if (Mask(Pos0.Y + Offset, X0, X1))
              std::fill(Frame.begin() + (Pos0.Y + Offset) * FrameSize.W + X0, Frame.begin() + (Pos0.Y + Offset) * FrameSize.W + X1 + 1, FrameColor);
            
            X0 = Pos0.X;
            X1 = Pos1.X;

            if (Mask(Pos1.Y - Offset, X0, X1))
              std::fill(Frame.begin() + (Pos1.Y - Offset) * FrameSize.W + X0, Frame.begin() + (Pos1.Y - Offset) * FrameSize.W + X1 + 1, FrameColor);
          }
        }

        /* Vertical lines */
        INT X0m, X1m;
        for (INT y = Pos0.Y + BorderW; y <= Pos1.Y - BorderW; y++)
        {
          if constexpr (IsBorderW1)
          {
            X0m = Pos0.X;
            X1m = Pos1.X;

            if (Mask(y, X0m, X1m))
            {
              if constexpr (IsFilled)
                std::fill(Frame.begin() + y * FrameSize.W + X0m, Frame.begin() + y * FrameSize.W + X1m + 1, SpaceColor);

              if (X0m == Pos0.X)
                PutPixelUnsafe(y * FrameSize.W + X0m, FrameColor);
              if (X1m == Pos1.X)
                PutPixelUnsafe(y * FrameSize.W + X1m, FrameColor);
            }
          }
          else
          {
            X0m = Pos0.X;
            X1m = Pos1.X;

            if (Mask(y, X0m, X1m))
            {
              if constexpr (IsFilled)
                std::fill(Frame.begin() + y * FrameSize.W + X0m, Frame.begin() + y * FrameSize.W + X1m + 1, SpaceColor);

              if (X0m <= Pos0.X + BorderW)
                std::fill(Frame.begin() + y * FrameSize.W + X0m, Frame.begin() + y * FrameSize.W + X0m + BorderW, FrameColor);
              if (X1m >= Pos1.X - BorderW)
                std::fill(Frame.begin() + y * FrameSize.W + X1m - BorderW + 1, Frame.begin() + y * FrameSize.W + X1m + 1, FrameColor);

            }
          }
        }
      } /* End of 'PutBar' function */

    /* The same functions with size argument */

    VOID PutBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD FrameColor, mask Mask )
    {
      // Bar isn't filled
      // Border is 1
      PutBar<0, 1>(Pos0, Pos1, FrameColor, 0, 0, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, mask Mask )
    {
      PutBar(Pos0, ivec2(Pos0.X + Size.W - 1, Pos0.Y + Size.H - 1), FrameColor, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, DWORD SpaceColor, mask Mask )
    {
      // Bar is filled
      // Border is 1
      PutBar<1, 1>(Pos0, ivec2(Pos0.X + Size.W - 1, Pos0.Y + Size.H - 1), FrameColor, SpaceColor, 0, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD FrameColor, INT BorderW, mask Mask )
    {
      // Bar isn't filled
      // Border isn't 1
      PutBar<0, 0>(Pos0, Pos1, FrameColor, 0, BorderW, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, INT BorderW, mask Mask )
    {
      PutBar(Pos0, ivec2(Pos0.X + Size.W - 1, Pos0.Y + Size.H - 1), FrameColor, BorderW, Mask);
    } /* End of 'PutBar' function */

    VOID PutBar( const ivec2 &Pos0, const isize2 &Size, DWORD FrameColor, DWORD SpaceColor, INT BorderW, mask Mask )
    {
      // Bar is filled
      // Border isn't 1
      PutBar<1, 0>(Pos0, ivec2(Pos0.X + Size.W - 1, Pos0.Y + Size.H - 1), FrameColor, SpaceColor, BorderW, Mask);
    } /* End of 'PutBar' function */

    /* Fill bar function */
    VOID FillBar( const ivec2 &Pos0, const ivec2 &Pos1, DWORD SpaceColor, mask Mask )
    {
      Mask = Mask.Intersect(FrameMask);

      if (!Mask.IsExist())
        return;

      /* Vertical lines */
      INT X0m, X1m;
      for (INT y = Pos0.Y; y <= Pos1.Y; y++)
      {
        X0m = Pos0.X;
        X1m = Pos1.X;

        if (Mask(y, X0m, X1m))
          std::fill(Frame.begin() + y * FrameSize.W + X0m, Frame.begin() + y * FrameSize.W + X1m + 1, SpaceColor);
      }
    } /* End of 'FillBar' function */

    VOID FillBar( const ivec2 &Pos0, const isize2 &Size, DWORD SpaceColor, mask Mask )
    {
      FillBar(Pos0, ivec2(Pos0.X + Size.W - 1, Pos0.Y + Size.H - 1), SpaceColor, Mask);
    } /* End of 'PutBar' function */
    
  }; /* End of 'render_2d' class */
   
} /* end of 'ui' namespace */

#endif // __2d_renderer_h_