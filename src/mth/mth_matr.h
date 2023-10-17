/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_matr.h
 * PURPOSE     : Raytracing project.
 *               Math matr functions module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

namespace mth
{
  template<typename Type>
  class matr
  {
    Type A[4][4];

  public:

    matr()
    {
    }

    matr(Type V00, Type V01, Type V02, Type V03,
         Type V10, Type V11, Type V12, Type V13,
         Type V20, Type V21, Type V22, Type V23,
         Type V30, Type V31, Type V32, Type V33) :
      A{
        {V00, V01, V02, V03},
        {V10, V11, V12, V13},
        {V20, V21, V22, V23},
        {V30, V31, V32, V33}
       }
    {
    }

    matr( const Type M[4][4] ) :
      A{
        {M[0][1], M[0][1], M[0][2], M[0][3]},
        {M[1][1], M[1][1], M[1][2], M[1][3]},
        {M[2][1], M[2][1], M[2][2], M[2][3]},
        {M[3][1], M[3][1], M[3][2], M[3][3]}
       }
    {
      /*
      A[0][0] = M[0][0], A[0][1] = M[0][1], A[0][2] = M[0][2], A[0][3] = M[0][3];
      A[1][0] = M[1][0], A[1][1] = M[1][1], A[1][2] = M[1][2], A[1][3] = M[1][3];
      A[2][0] = M[2][0], A[2][1] = M[2][1], A[2][2] = M[2][2], A[2][3] = M[2][3];
      A[3][0] = M[3][0], A[3][1] = M[3][1], A[3][2] = M[3][2], A[3][3] = M[3][3];
      */
    }

    matr( const matr &M ) :
      A{
        {M.A[0][0], M.A[0][1], M.A[0][2], M.A[0][3]},
        {M.A[1][0], M.A[1][1], M.A[1][2], M.A[1][3]},
        {M.A[2][0], M.A[2][1], M.A[2][2], M.A[2][3]},
        {M.A[3][0], M.A[3][1], M.A[3][2], M.A[3][3]}
       }
    {
      /*
      A[0][0] = M[0][0], A[0][1] = M[0][1], A[0][2] = M[0][2], A[0][3] = M[0][3];
      A[1][0] = M[1][0], A[1][1] = M[1][1], A[1][2] = M[1][2], A[1][3] = M[1][3];
      A[2][0] = M[2][0], A[2][1] = M[2][1], A[2][2] = M[2][2], A[2][3] = M[2][3];
      A[3][0] = M[3][0], A[3][1] = M[3][1], A[3][2] = M[3][2], A[3][3] = M[3][3];
      */
    }

    /* Multiply operator function.
     * ARGUMENTS:
     *   - other matr:
     *       const matr &M;
     * RETURNS:
     *   (matr) - out matriX.
     */
    matr operator*( const matr &M ) const
    {
      matr Out(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          for (INT k = 0; k < 4; k++)
            Out.A[i][j] += A[i][k] * M.A[k][j];

      return Out;
    } /* End of '*' operator function */

    /* '!' make determ in matriX 4X4 function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (DBL) - resoult determ.
     */
    DBL operator!( void ) const
    {
      return
        A[0][0] * Determ3X3(A[1][1], A[1][2], A[1][3],
                            A[2][1], A[2][2], A[2][3],
                            A[3][1], A[3][2], A[3][3]) -
        A[0][1] * Determ3X3(A[1][0], A[1][2], A[1][3],
                            A[2][0], A[2][2], A[2][3],
                            A[3][0], A[3][2], A[3][3]) +
        A[0][2] * Determ3X3(A[1][0], A[1][1], A[1][3],
                            A[2][0], A[2][1], A[2][3],
                            A[3][0], A[3][1], A[3][3]) -
        A[0][3] * Determ3X3(A[1][0], A[1][1], A[1][2],
                            A[2][0], A[2][1], A[2][2],
                            A[3][0], A[3][1], A[3][2]);
    } /* End of '!' operator function */

    /* 'MatrTranspose' swap all nums by center function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) - resoult matriX.
     */
    matr Transpose( void ) const
    {
      matr Out;

      for (INT i : A)
        for (INT j : A)
          Out.A[i][j] = A[j][i];

      return Out;
    } /* End of 'Transpose' function */

    /* 'MatrDeterm3X3' make determ in matriX 3X3 function.
     * ARGUMENTS:
     *   - nums of matriX.
     *     DBL
     *       A11, A12, A13,
     *       A21, A22, A23,
     *       A31, A32, A33;
     * RETURNS:
     *   (DBL) - resoult determ.
     */
    Type Determ3X3( Type A11, Type A12, Type A13,
                   Type A21, Type A22, Type A23,
                   Type A31, Type A32, Type A33 ) const
    {
      return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
             A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
    } /* End of 'MatrDeterm3X3' function */

    /* 'Identity' makes default matr function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr &) - self matr referense.
     */
    matr & Identity( void )
    {
      return this->matr(1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);
    } /* End of 'Transpose' function */

    /* 'MatrInverse' make inverse matriX from another matriX function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) - resoult inversed matr.
     */
    matr Inverse( void ) const
    {
      DBL det = !*this;
      INT s[] = {1, -1}, P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
 
      if (det == 0)
        return matr();
 
      matr Out;
      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          Out.A[j][i] =
            s[(i + j) % 2] *
            Determ3X3(A[P[i][0]][P[j][0]], A[P[i][0]][P[j][1]], A[P[i][0]][P[j][2]],
                      A[P[i][1]][P[j][0]], A[P[i][1]][P[j][1]], A[P[i][1]][P[j][2]],
                      A[P[i][2]][P[j][0]], A[P[i][2]][P[j][1]], A[P[i][2]][P[j][2]]) / det;
      return Out;
    } /* End of 'Inverse' function */
    
    /* 'Translate' make translate matr from vec function.
     * ARGUMENTS:
     *   - vector.
     *       const vec3 &Vec;
     * RETURNS:
     *   (MATR) - result matriX.
     */
    static matr Translate( const vec3<Type> &Vec )
    {
      return matr(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  Vec.X, Vec.Y, Vec.Z, 1);
    } /* End of 'Translate' function */

    /* 'MatrScale' retruns matr from vec function.
     * ARGUMENTS:
     *   - scale vector.
     *     const vec3<Type> &Scale;
     * RETURNS:
     *   (matr) - resoult matriX.
     */
    static matr Scale( const vec3<Type> &Scale )
    {
      return matr(Scale.X, 0, 0, 0,
                  0, Scale.Y, 0, 0,
                  0, 0, Scale.Z, 0,
                  0, 0, 0, 1);
    } /* End of 'Scale' function */

    /* 'RotateX' make rotate matr function.
     * ARGUMENTS:
     *   - roteted angle.
     *       const DBL &AngleInDegree;
     * RETURNS:
     *   (MATR) - resoult matriX.
     */
    static matr RotateX( const DBL &AngleInDegree )
    {
      return matr(1, 0, 0, 0,
                  0, cos(AngleInDegree), sin(AngleInDegree), 0,
                  0, -sin(AngleInDegree), cos(AngleInDegree), 0,
                  0, 0, 0, 1);
    } /* End of 'RotateX' function */

    /* 'RotateY' make rotate matr function.
     * ARGUMENTS:
     *   - roteted angle.
     *       const DBL &AngleInDegree;
     * RETURNS:
     *   (MATR) - resoult matriX.
     */
    static matr RotateY( const DBL &AngleInDegree )
    {
      return matr(cos(AngleInDegree), 0, -sin(AngleInDegree), 0,
                  0, 1, 0, 0,
                  sin(AngleInDegree), 0, cos(AngleInDegree), 0,
                  0, 0, 0, 1);
    } /* End of 'RotateY' function */

    /* 'RotateZ' make rotate matr function.
     * ARGUMENTS:
     *   - roteted angle.
     *       const DBL &AngleInDegree;
     * RETURNS:
     *   (MATR) - resoult matriX.
     */
    static matr RotateZ( const DBL &AngleInDegree )
    {
      return matr(cos(AngleInDegree), sin(AngleInDegree), 0, 0,
                  -sin(AngleInDegree), cos(AngleInDegree), 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
    } /* End of 'RotateZ' function */

    /* 'Rotate' make rotate matr function.
     * ARGUMENTS:
     *   - roteted angle.
     *       const DBL &AngleInDegree;
     *   - rotating aXis vector.
     *       const vec3<Type> &AXis;
     * RETURNS:
     *   (matr) - result matriX.
     */
    static matr Rotate( const DBL &AngleInDegree, const vec3<Type> &AXis )
    {
      DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
      vec3<Type> A = AXis.Normalise();

      return matr(c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) - A.Z * s, A.X * A.Z * (1 - c) + A.Y * s, 0,                                  // error on the site +/- 
                  A.X * A.Y * (1 - c) + A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) - A.X * s, 0,
                  A.X * A.Z * (1 - c) - A.Y * s, A.Y * A.Z * (1 - c) + A.X * s, c + A.Z * A.Z * (1 - c), 0,
                  0, 0, 0, 1);
    } /* End of 'Rotate' function */

    /* Multiply vector on matr operator function.
     * ARGUMENTS:
     *   - vector:
     *       const vec3<Type> &V;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3<Type> TransformVector( const vec3<Type> &V ) const
    {
      Type w = V.X * A[0][3] + V.Y * A[1][3] + V.Z * A[2][3] + A[3][3];

      return vec3<Type>((V.X * A[0][0] + V.Y * A[1][0] + V.Z * A[2][0] + A[3][0]) / w,
                        (V.X * A[0][1] + V.Y * A[1][1] + V.Z * A[2][1] + A[3][1]) / w,
                        (V.X * A[0][2] + V.Y * A[1][2] + V.Z * A[2][2] + A[3][2]) / w);
    } /* End of 'TransformVector' operator function */

    /* Multiply vector like point on matr operator function.
     * ARGUMENTS:
     *   - vector:
     *       const vec3<Type> &V;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3<Type> TransformPoint( const vec3<Type> &V ) const
    {
      return VecSet(V.X * A[0][0] + V.Y * A[1][0] + V.Z * A[2][0] + A[3][0],
                    V.X * A[0][1] + V.Y * A[1][1] + V.Z * A[2][1] + A[3][1],
                    V.X * A[0][2] + V.Y * A[1][2] + V.Z * A[2][2] + A[3][2]);
    } /* End of 'TransformPoint' operator function */

    /* Multiply normal vector like point on matr operator function.
     * ARGUMENTS:
     *   - normal vector:
     *       const vec3<Type> &V;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3<Type> TransformNormal( const vec3<Type> &N ) const
    {
      return VecSet(N.X * A[0][0] + N.Y * A[0][1] + N.Z * A[0][2],
                    N.X * A[1][0] + N.Y * A[1][1] + N.Z * A[1][2],
                    N.X * A[2][0] + N.Y * A[2][1] + N.Z * A[2][2]);
    } /* End of 'TransformNormal' operator function */

  };
}

#endif //__mth_matr_h_ 


/* 
TransformNormal
Transform4X4
*/