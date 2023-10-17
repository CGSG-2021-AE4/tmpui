/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_noise.h
 * PURPOSE     : Raytracing project.
 *               Noise math handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 29.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


#ifndef __mth_noise_h_
#define __mth_noise_h_

#include "mthdef.h"

/* noise */
#define TAB_BITS 8
#define TAB_SIZE (1 << TAB_BITS)
#define TAB_MASK (TAB_SIZE - 1)

#define INDEX1(X) ((INT)Perm[(X) & TAB_MASK])
#define INDEX2(X, Y) ((INT)Perm[(X + INDEX1(Y)) & TAB_MASK])
#define INDEX3(X, Y, Z) ((INT)Perm[(Y + INDEX2(X, Z)) & TAB_MASK])

namespace mth
{
  class noise
  {
    DBL TabNoise[TAB_SIZE];
    FLT Perm[TAB_SIZE];
  public:

    /* noise table init function
     * ARGUENTS: None.
     * RETURNS: None.
     */
    noise()
    {
      INT i;

      for (i = 0; i < TAB_SIZE; i++)
        TabNoise[i] = (DBL)rand() / RAND_MAX;

      for (i = 0; i < TAB_SIZE; i++)
        Perm[i] = i;

      for (i = 0; i < TAB_SIZE * 8; i++)
      {
        INT 
          a = rand() & TAB_MASK,
          b = rand() & TAB_MASK;
        DBL tmp = Perm[a];

        Perm[a] = Perm[b];
        Perm[b] = tmp;
      }
    } /* End of 'NoiseInit' function */

    /* noise table update function
     * ARGUENTS:
     *   - Noise arg:
     *       DBL X;
     * RETURNS:
     *   (DBL) noise value.
     */
    DBL Noise1D( const DBL &X )
    {
      INT ix = (INT)floor(X), ix1;
      DBL fx;

      fx = X - ix;
      fx = (3 - 2 * fx) * fx * fx;
      ix &= TAB_MASK;
      ix1 = (ix + 1) & TAB_MASK;
      return TabNoise[ix] * (1 - fx) + TabNoise[ix1] * fx;
    } /* End of 'Noise1D' function */

    /* noise 2d table update function
     * ARGUENTS:
     *   - Noise args:
     *       DBL X, Y;
     * RETURNS:
     *   (DBL) noise value.
     */
    DBL Noise2D( const DBL &X, const DBL &Y )
    {
      INT ix = (INT)floor(X), ix1, iy = (INT)floor(Y), iy1;
      DBL fx, fy;

      fx = X - ix;
      fx = (3 - 2 * fx) * fx * fx;
  
      fy = Y - iy;
      fy = (3 - 2 * fy) * fy * fy;

      ix &= TAB_MASK;
      ix1 = (ix + 1) & TAB_MASK;
  
      iy &= TAB_MASK;
      iy1 = (iy + 1) & TAB_MASK;


      return
        TabNoise[INDEX2(ix,  iy  )] * (1 - fx) * (1 - fy) +
        TabNoise[INDEX2(ix1,  iy )] * fx * (1 - fy) +
        TabNoise[INDEX2(ix, iy1  )] * (1 - fx) * fy +
        TabNoise[INDEX2(ix1, iy1 )] * fx * fy;
    } /* End of 'Noise1D' function */

    /* noise 3d table update function
     * ARGUENTS:
     *   - Noise args:
     *       DBL X, Y, Z;
     * RETURNS:
     *   (DBL) noise value.
     */
    DBL Noise3D( const DBL &X, const DBL &Y, const DBL &Z )
    {
      INT ix = (INT)floor(X), ix1, iy = (INT)floor(Y), iy1, iz = (INT)floor(Z), iz1;
      DBL fx, fy, fz;

      fx = X - ix;
      fx = (3 - 2 * fx) * fx * fx;
  
      fy = Y - iy;
      fy = (3 - 2 * fy) * fy * fy;

      fz = Z - iz;
      fz = (3 - 2 * fz) * fz * fz;

      ix &= TAB_MASK;
      ix1 = (ix + 1) & TAB_MASK;
  
      iy &= TAB_MASK;
      iy1 = (iy + 1) & TAB_MASK;

      iz &= TAB_MASK;
      iz1 = (iz + 1) & TAB_MASK;

      return
        TabNoise[INDEX3(ix , iy , iz  )] * (1 - fx) * (1 - fy) * (1 - fz) +

        TabNoise[INDEX3(ix , iy , iz1 )] * (1 - fx) * (1 - fy) * fz +
        TabNoise[INDEX3(ix1, iy , iz  )] * (1 - fy) * (1 - fz) * fx +
        TabNoise[INDEX3(ix , iy1, iz  )] * (1 - fz) * (1 - fx) * fy +

        TabNoise[INDEX3(ix , iy1, iz1 )] * (1 - fx) * fy * fz +
        TabNoise[INDEX3(ix1, iy , iz1 )] * (1 - fy) * fz * fx +
        TabNoise[INDEX3(ix1, iy1, iz  )] * (1 - fz) * fx * fy +

        TabNoise[INDEX3(ix1, iy1, iz1 )] * fx * fy * fz;
    } /* End of 'Noise3D' function */

    /* noise turb update function
     * ARGUENTS:
     *   - Noise arg:
     *       DBL X;
     *   - number of octaves:
     *       INT Octaves;
     * RETURNS:
     *   (DBL) turb value.
     */
    DBL NoiseTurb1D( DBL X, const INT &Octaves )
    {
      INT frac = 1, i;
      DBL val = 0;

      for (i = 0; i < Octaves; i++)
      {
        val += Noise1D(X) / frac;
        X = (X + 23.47) * 2;
        frac *= 2;
      }

      return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
    } /* end of 'NoiseTurb1D' function */

    /* noise turb  2dupdate function
     * ARGUENTS:
     *   - Noise args:
     *       DBL X, Y;
     *   - number of octaves:
     *       INT Octaves;
     * RETURNS:
     *   (DBL) turb value.
     */
    DBL NoiseTurb2D( DBL X, DBL Y, const INT &Octaves )
    {
      INT frac = 1, i;
      DBL val = 0;

      for (i = 0; i < Octaves; i++)
      {
        val += Noise2D(X, Y) / frac;
        X = (X + 23.47) * 2;
        Y = (Y + 25.83) * 2;
        frac *= 2;
      }

      return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
    } /* End of 'NoiseTurb2D' function */

    /* noise turb  3d update function
     * ARGUENTS:
     *   - Noise args:
     *       DBL X, Y, Z;
     *   - number of octaves:
     *       INT Octaves;
     * RETURNS:
     *   (DBL) turb value.
     */
    DBL NoiseTurb3D( DBL X, DBL Y, DBL Z, const INT &Octaves )
    {
      INT frac = 1, i;
      DBL val = 0;

      for (i = 0; i < Octaves; i++)
      {
        val += Noise3D(X, Y, Z) / frac;
        X = (X + 23.47) * 2;
        Y = (Y + 25.83) * 2;
        Z = (Z + 24.28) * 2;
        frac *= 2;
      }

      return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
    } /* End of 'NoiseTurb3D' function */
  };
}


#endif //__mth_matr_h_

/* END OF 'mth_noise.h' FILE */