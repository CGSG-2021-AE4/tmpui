/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : frame.h
 * PURPOSE     : Raytracing project.
 *               Frame dependent handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 24.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <string>
#include <fstream>

#pragma pack(push, 1)
#include <tgahead.h>
#pragma pack(pop)

#ifndef __frame_h_
#define __frame_h_

namespace tmp
{
  class frame
  {
  public:

    DWORD *Image;
    bool IsArrayAvailable = 0;

    INT W, H;

    /* Frame construct function */
    frame() : W(300), H(200)
    {
      Image = new DWORD[W * H];
      IsArrayAvailable = 1;
    } /* End of 'frame' function */

    /* Frame construct function */
    frame( INT NewW, INT NewH = 200 ) : W(NewW), H(NewH)
    {
      Image = new DWORD[W * H];
      IsArrayAvailable = 1;
    } /* End of 'frame' function */

    /* Frame deconstruct function */
    ~frame()
    {
      IsArrayAvailable = 0;
      delete Image;
    }  /* End of 'frame' function */

    /* Frame resize function.
     * ARGUMENTS:
     *   - new W, H:
     *       const INT &NewW, &NewH;
     * RETURNS: None.
     */
    void Resize( const INT &NewW, const INT &NewH )
    {
      IsArrayAvailable = 0;
      delete Image;
      W = NewW;
      H = NewH;
      Image = new DWORD[W * H];
      IsArrayAvailable = 1;
    } /* End of 'Resize' function */

    /* Put pixel to frame function.
     * ARGUMENTS:
     *   - pixel x, y:
     *       INT &x, &y;
     *   - pixel Color:
     *       const DWORD &Color;
     * RETURNS: None.
     */
    void PutPixel( INT &x, INT &y, const DWORD &Color )
    {
      if (IsArrayAvailable)
        if (x > 0 && y > 0 && x < W && y < H)
          Image[y * W + x] = Color;
    } /* End of 'PutPixel' function */

    /* Frame to display function.
     * ARGUMENTS:
     *   - display DC handle:
     *       HDC &hDC;
     *   - display bit map:
     *        HBITMAP &hBm;
     * RETURNS: None.
     */
    void Display( HDC &hDC, HBITMAP &hBm )
    {
      if (IsArrayAvailable)
      {
        BITMAPINFOHEADER bmih = {sizeof(BITMAPINFOHEADER), W, -H, 1, 32};
    
        SetDIBits(hDC, hBm, 0, H, Image, (BITMAPINFO *)&bmih, DIB_RGB_COLORS);
      }
    } /* End of 'Display' function */

    /* Frame save to file function.
     * ARGUMENTS:
     *   - out file name:
     *       const std::string &OutFileName;
     * RETURNS: None.
     */
    void Save( const std::string &OutFileName )
    {
      if (!IsArrayAvailable)
        return;

      CHAR Buf[300];

      SYSTEMTIME St;

      GetLocalTime(&St);

      sprintf_s(Buf, "%s_%04d%02d%02d_%02d%02d%02d_%03d.tga", OutFileName.c_str(), St.wYear, St.wMonth, St.wDay, St.wHour, St.wMinute, St.wSecond, St.wMilliseconds);

      std::fstream F(Buf, std::fstream::out | std::fstream::binary);

      if (!F)
        return;

      CHAR Descr[] = "AE4_NEWTONF";

      tgaFILEHEADER Header =
      {
        sizeof(Descr),
        0, 2,
        0, 0, 0,
        0, 0, (WORD)W, (WORD)H,
        32, 0x28
      };

      F.write((const CHAR *)&Header, sizeof(Header));

      F.write(Descr, sizeof(Descr));
      F.write((const CHAR *)Image, W * H * sizeof(DWORD));

      F.write(TGA_EXT_SIGNATURE, sizeof(TGA_EXT_SIGNATURE));
    } /* End of 'Save' function */

    /* Frame clear function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void Clear( void )
    {
      memset(Image, 0, W * H * sizeof(DWORD));
    } /* End of 'Clear' function */
    
    friend class rt;
  };

}

#endif //__frame_h_