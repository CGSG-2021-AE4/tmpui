/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Raytracing project.
 *               Math vec2 functions  module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <cstdlib>

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

namespace mth
{
  /* vec2 template class */
  template<typename Type>
  class vec2
  {
  public:
    
    Type X, Y; // X, Y components of vec2 type

    /* vec2 default construct function */
    vec2() : X(0), Y(0)
    {
    } /* End of 'vec2' construct function */

    /* vec2 construct function from 2 components */
    vec2( const Type &NewX, const Type &NewY ) : X(NewX), Y(NewY)
    {
    } /* End of 'vec2' construct function */

    /* vec2 default construct function from 1 component */
    vec2 ( const Type &A ) : X(A), Y(A)
    {
    } /* End of 'vec2' construct function */

    /* Eual operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2 &) - self vector referense.
     */
    vec2 & operator=( const vec2 &Vec )
    {
      X = Vec.X;
      Y = Vec.Y;

      return *this;
    } /* End of '=' operator function */

    /* Module operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (DBL) - out module.
     */
    DBL operator!( void ) const
    {
      return sqrt(X * X + Y * Y);
    } /* End of '!' operator function */

    /* Multiply operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator*( const vec2 &Vec ) const
    {
      return vec2(X * Vec.X, Y * Vec.Y);
    } /* End of '*' operator function */


    /* Add operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator+( const vec2 &Vec ) const
    {
      return vec2(X + Vec.X, Y + Vec.Y);
    } /* End of '+' operator function */

    /* Subst operator function.
     * ARGUMENTS:
     *   - other vec:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator-( const vec2 &Vec ) const
    {
      return vec2(X - Vec.X, Y - Vec.Y);
    } /* End of '-' operator function */

    /* Negative operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator-( void ) const
    {
      return vec2(-X, -Y);
    } /* End of '-' operator function */

    /* Multiply on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const Type &N;
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator*( const Type &N ) const
    {
      return vec2(X * N, Y * N);
    } /* End of '*' operator function */

    /* Scalar multiply on num operator function.
     * ARGUMENTS:
     *   - in vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (Type) - out num.
     */
    Type operator&( const vec2 &Vec ) const
    {
      return X * Vec.X + Y * Vec.Y;
    } /* End of '*' operator function */

    /* Vector multiply on num operator function.
     * ARGUMENTS:
     *   - in vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (Type) - Z component of crossed vector.
     */
    Type operator%( const vec2 &Vec ) const
    {
      return X * Vec.Y - Y * Vec.X;
    } /* End of '%' operator function */

    /* Division on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const Type &N;
     * RETURNS:
     *   (vec2) - out vector.
     */
    vec2 operator/( const Type &N ) const
    {
      return vec2(X / N, Y / N);
    } /* End of '/' operator function */

    /* Add equncy operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2 &) - this vector referense.
     */
    vec2 & operator+=( const vec2 &Vec )
    {
      *this = *this + Vec;

      return *this;

    } /* End of '+=' operator function */

    /* Subtract equncy operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (vec2 &) - this vector referense.
     */
    vec2 & operator-=( const vec2 &Vec )
    {
      *this = *this - Vec;

      return *this;
    } /* End of '-=' operator function */

    /* Mutiply equncy operator function.
     * ARGUMENTS:
     *   - adding vector:
     *       const Type &Num;
     * RETURNS:
     *   (vec2 &) - this vector referense.
     */
    vec2 & operator*=( const Type &Num )
    {
      *this = *this * Num;

      return *this;
    } /* End of '*=' operator function */

     /* Is equal operator function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2<Type> &V;
     * RETURNS:
     *   (BOOL) - is equal.
     */
    BOOL operator==( const vec2<Type> &V ) const
    {
      return X == V.X && Y == V.Y;
    } /* End of '==' operator function */
    
    /* Is not equal operator function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2<Type> &V;
     * RETURNS:
     *   (BOOL) - is not equal.
     */
    BOOL operator!=( const vec2<Type> &V ) const
    {
      return X != V.X || Y != V.Y;
    } /* End of '!=' operator function */
    
    /* Zero vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2 &) - self vector referense.
     */
    vec2 & Zero( void )
    {
      X = Y = 0;

      return *this;
    } /* End of 'Zero' operator function */

    /* Random with max function.
     * ARGUMENTS:
     *   - max random:
     *       const INT &RndMax;
     * RETURNS:
     *   (vec2 &) - self vector referense.
     */
    vec2 & Rnd0( const INT &RndMax )
    {
      X = rand() % RndMax, Y = rand() % RndMax;

      return *this;
    } /* End of 'Rnd' operator function */

    /* Random from -1 to 1 function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2 &) - self vector referense.
     */
    vec2 & Rnd1( void )
    {
      X = (2.0 * rand()) / RAND_MAX - 1, Y = (2.0 * rand()) / RAND_MAX - 1;
      return *this;
    } /* End of 'Rnd' operator function */

    /* Returns normalise vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - normalised vector.
     */
    vec2 & Normalise( void ) const
    {
      DBL len = sqrt(*this & *this); // ?
 
      if (len == 1 || len == 0)
        return *this;
      return *this / len;
    } /* End of 'Normalise' operator function */

    /* Normalising self vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - self vector referense.
     */
    vec2 & Normaliseing( void )
    {
      DBL len = sqrt(*this & *this); // ?
 
      if (len == 1 || len == 0)
        return *this;
      return *this /= len;
    } /* End of 'Normaliseing' operator function */

    /* Returns vector lenght in cube function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (Type) - vector len.
     */
    Type Length2( void ) const
    {
      return X * X + Y * Y;
    } /* End of 'Length2' operator function */

    /* Returns distance between two vectors function.
     * ARGUMENTS:
     *   - second vector:
     *       const vec2 &Vec;
     * RETURNS:
     *   (DBL) - dsitance len.
     */
    DBL Distance( const vec2 &Vec ) const
    {
      return !(*this - Vec);
    } /* End of 'Length2' operator function */

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
      default:
        return X;
      }
    } /* End of 'Length2' operator function */

    template<class Type1>
      friend class matr;
  };
}
#endif // __mth_vec2_h_


/*
operator Type *
*/