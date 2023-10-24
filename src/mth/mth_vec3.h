/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_vec3.h
 * PURPOSE     : Raytracing project.
 *               Math vec3 functions module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <cstdlib>

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

namespace mth
{
  /* vec3 template class */
  template<typename Type>
  class vec3
  {
  public:

    Type X, Y, Z; // X, Y, Z components of vec3 type

    /* vec3 default construct function */
    vec3() : X(0), Y(0), Z(0)
    {
    } /* End of 'vec3' construct function */

    /* vec3 construct function from 3 components */
    vec3( const Type &NewX, const Type &NewY, const Type &NewZ ) : X(NewX), Y(NewY), Z(NewZ)
    {
    } /* End of 'vec3' construct function */

    /* vec3 default construct function from 1 component */
    vec3 ( const Type &A ) : X(A), Y(A), Z(A)
    {
    } /* End of 'vec3' construct function */

    /* Eual operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3 &) - self vector referense.
     */
    vec3 & operator=( const vec3 &Vec )
    {
      X = Vec.X;
      Y = Vec.Y;
      Z = Vec.Z;

      return *this;
    } /* End of '=' operator function */

    /* Module operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (DBL) - out module.
     */
    DBL operator!( void ) const
    {
      return sqrt(X * X + Y * Y + Z * Z);
    } /* End of '!' operator function */

    /* Multiply on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const DBL &N;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator*( const Type &N ) const
    {
      return vec3(X * N, Y * N, Z * N);
    } /* End of '*' operator function */

    /* Multiply operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator*( const vec3 &Vec ) const
    {
      return vec3(X * Vec.X, Y * Vec.Y, Z * Vec.Z);
    } /* End of '*' operator function */

    /* Multiply on matr operator function.
     * ARGUMENTS:
     *   - matr:
     *       const matr &M;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator*( matr<Type> &M ) const
    {
      Type w = X * M.A[0][3] + Y * M.A[1][3] + Z * M.A[2][3] + M.A[3][3];

      return vec3((X * M.A[0][0] + Y * M.A[1][0] + Z * M.A[2][0] + M.A[3][0]) / w,
                  (X * M.A[0][1] + Y * M.A[1][1] + Z * M.A[2][1] + M.A[3][1]) / w,
                  (X * M.A[0][2] + Y * M.A[1][2] + Z * M.A[2][2] + M.A[3][2]) / w);
    } /* End of '*' operator function */

    /* Add operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator+( const vec3 &Vec ) const
    {
      return vec3(X + Vec.X, Y + Vec.Y, Z + Vec.Z);
    } /* End of '+' operator function */

    /* Subst operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator-( const vec3 &Vec ) const
    {
      return vec3(X - Vec.X, Y - Vec.Y, Z - Vec.Z);
    } /* End of '-' operator function */

    /* Negative operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator-( void ) const
    {
      return vec3(-X, -Y, -Z);
    } /* End of '-' operator function */

    

    /* Scalar multiply on num operator function.
     * ARGUMENTS:
     *   - in vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (DBL) - out num.
     */
    DBL operator&( const vec3 &Vec ) const
    {
      return X * Vec.X + Y * Vec.Y + Z * Vec.Z;
    } /* End of '*' operator function */

    /* Vector multiply on num operator function.
     * ARGUMENTS:
     *   - in vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator%( const vec3 &Vec ) const
    {
      return vec3(Y * Vec.Z - Z * Vec.Y, Z * Vec.X - X * Vec.Z, X * Vec.Y - Y * Vec.X);
    } /* End of '%' operator function */

    /* Division on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const Type &N;
     * RETURNS:
     *   (vec3) - out vector.
     */
    vec3 operator/( const Type &N ) const
    {
      return vec3(X / N, Y / N, Z / N);
    } /* End of '/' operator function */

    /* Add equncy operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3 &) - self vector referense.
     */
    vec3 & operator+=( const vec3 &Vec )
    {
      *this = *this + Vec;

      return *this;

    } /* End of '+=' operator function */

    /* Subtract equncy operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (vec3 &) - self vector referense.
     */
    vec3 & operator-=( const vec3 &Vec )
    {
      *this = *this - Vec;

      return *this;
    } /* End of '-=' operator function */

    /* Mutiply equncy operator function.
     * ARGUMENTS:
     *   - multipling num:
     *       const Type &Num;
     * RETURNS:
     *   (vec3 &) - self vector referense.
     */
    vec3 & operator*=( const Type &Num )
    {
      *this = *this * Num;
      return *this;
    } /* End of '*=' operator function */

    /* Devide equncy operator function.
     * ARGUMENTS:
     *   - deviding num:
     *       const Type &Num;
     * RETURNS:
     *   (vec3 &) - self vector referense.
     */
    vec3 & operator/=( const Type &Num )
    {
      *this = *this / Num;
      return *this;
    } /* End of '/=' operator function */

    /* Is equal operator function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3<Type> &V;
     * RETURNS:
     *   (BOOL) - is equal.
     */
    BOOL operator==( const vec3<Type> &V )
    {
      return X == V.X && Y == V.Y && Z == V.Z;
    } /* End of '==' operator function */
    
    /* Is not equal operator function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3<Type> &V;
     * RETURNS:
     *   (BOOL) - is not equal.
     */
    BOOL operator!=( const vec3<Type> &V )
    {
      return X != V.X || Y != V.Y || Z != V.Z;
    } /* End of '!=' operator function */

    /* Zero vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3).
     */
    static vec3 Zero( void )
    {
      return {0, 0, 0};
    } /* End of 'Zero' function */

    /* Random with max function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - out vector.
     */
    static vec3 Rnd0( void )
    {
      return {(Type)rand() / RAND_MAX, (Type)rand() / RAND_MAX, (Type)rand() / RAND_MAX};
    } /* End of 'Rnd0' function */

    /* Random from -1 to 1 function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - out vector.
     */
    static vec3 Rnd1( void )
    {
      return {(2.0 * rand()) / RAND_MAX - 1, (2.0 * rand()) / RAND_MAX - 1, (2.0 * rand()) / RAND_MAX - 1};
    } /* End of 'Rnd1' function */

    /* Returns normalise vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - normalised vector.
     */
    vec3 Normalise( void ) const
    {
      DBL len = sqrt(*this & *this);
 
      if (len == 1 || len == 0)
        return *this;
      return *this / len;
    } /* End of 'Normalise' operator function */

    /* Normalising self vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - self vector referense.
     */
    vec3 & Normalising( void )
    {
      DBL len = sqrt(*this & *this);
 
      if (len == 1 || len == 0)
        return *this;
      return *this /= len;
    } /* End of 'Normaliseing' operator function */

    /* Returns vector lenght in cube function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (DBL) - vector len.
     */
    DBL Length2( void ) const
    {
      return X * X + Y * Y + Z * Z;
    } /* End of 'Length2' operator function */

    /* Returns distance between two vectors function.
     * ARGUMENTS:
     *   - second vector:
     *       const vec3 &Vec;
     * RETURNS:
     *   (DBL) - dsitance len.
     */
    DBL Distance( const vec3 &Vec ) const
    {
      return !(*this - Vec);
    } /* End of 'Distance' operator function */

    /* '[]' operator function.
     * ARGUMENTS:
     *   - vector comp id:
     *       const INT &Id;
     * RETURNS:
     *   (Type) - vector component.
     */
    Type & operator[]( const INT &Id )
    {
      switch (Id)
      {
      case 0:
        return X;
      case 1:
        return Y;
      case 2:
        return Z;
      default:
        return X;
      }
    } /* End of 'Length2' operator function */

    

    template<class Type1>
      friend class matr;
  };
}
#endif // __mth_vec3_h_


/*
operator Type *
*/