/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Skinning project.
 *               Math size functions  module.
 * PROGRAMMER  : CGSG-FallCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 30.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <cstdlib>

#ifndef __mth_size_h_
#define __mth_size_h_

namespace mth
{
  /* size template class */
  template<typename Type>
  class size
  {
  public:
    Type W, H; // W, H components of size type

    /* size default construct function */
    size() : W(0), H(0)
    {
    } /* End of 'size' construct function */

    /* size construct function from 2 components */
    size( const Type &NewW, const Type &NewH ) : W(NewW), H(NewH)
    {
    } /* End of 'size' construct function */

    /* size default construct function from 1 component */
    size( const Type &A ) : W(A), H(A)
    {
    } /* End of 'size' construct function */

    /* Eual operator function.
     * ARGUMENTS:
     *   - adding Size:
     *       const size &Size;
     * RETURNS:
     *   (size &) - self Size referense.
     */
    size & operator=( const size &Size )
    {
      W = Size.W;
      H = Size.H;

      return *this;
    } /* End of '=' operator function */

    /* Module operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (FLT) - out module.
     */
    FLT operator!( void ) const
    {
      return sqrt(W * W + H * H);
    } /* End of '!' operator function */

    /* Multiply operator function.
     * ARGUMENTS:
     *   - other Size:
     *       const size &Size;
     * RETURNS:
     *   (size) - out Size.
     */
    size operator*( const size &Size ) const
    {
      return size(W * Size.W, H * Size.H);
    } /* End of '*' operator function */

    /* Add operator function.
     * ARGUMENTS:
     *   - other size:
     *       const size &Size;
     * RETURNS:
     *   (size) - out size.
     */
    size operator+( const size &Size ) const
    {
      return size(W + Size.W, H + Size.H);
    } /* End of '+' operator function */

    /* Subst operator function.
     * ARGUMENTS:
     *   - other Size:
     *       const size &Size;
     * RETURNS:
     *   (size) - out Size.
     */
    size operator-( const size &Size ) const
    {
      return size(W - Size.W, H - Size.H);
    } /* End of '-' operator function */

    /* Negative operator function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (size) - out Size.
     */
    size operator-( void ) const
    {
      return size(-W, -H);
    } /* End of '-' operator function */

    /* Multiply on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const Type &N;
     * RETURNS:
     *   (size) - out Size.
     */
    size operator*( const Type &N ) const
    {
      return size(W * N, H * N);
    } /* End of '*' operator function */

    /* Scalar multiply on num operator function.
     * ARGUMENTS:
     *   - in Size:
     *       const size &Size;
     * RETURNS:
     *   (Type) - out num.
     */
    Type operator&( const size &Size ) const
    {
      return W * Size.W + H * Size.H;
    } /* End of '*' operator function */

    /* Size multiply on num operator function.
     * ARGUMENTS:
     *   - in Size:
     *       const size &Size;
     * RETURNS:
     *   (Type) - Z component of crossed Size.
     */
    Type operator%( const size &Size ) const
    {
      return W * Size.H - H * Size.W;
    } /* End of '%' operator function */

    /* Division on num operator function.
     * ARGUMENTS:
     *   - in number:
     *       const Type &N;
     * RETURNS:
     *   (size) - out Size.
     */
    size operator/( const Type &N ) const
    {
      return size(W / N, H / N);
    } /* End of '/' operator function */

    /* Add equncy operator function.
     * ARGUMENTS:
     *   - adding Size:
     *       const size &Size;
     * RETURNS:
     *   (size &) - this Size referense.
     */
    size & operator+=( const size &Size )
    {
      *this = *this + Size;

      return *this;

    } /* End of '+=' operator function */

    /* Is equal operator function.
     * ARGUMENTS:
     *   - other size:
     *       const size &Size;
     * RETURNS:
     *   (BOOL) - result.
     */
    BOOL operator==( const size &Size )
    {
      return W == Size.W && H == Size.H;
    } /* End of '==' operator function */

    /* Is not equal operator function.
     * ARGUMENTS:
     *   - other size:
     *       const size &Size;
     * RETURNS:
     *   (BOOL) - result.
     */
    BOOL operator!=( const size &Size )
    {
      return W != Size.W || H != Size.H;
    } /* End of '!=' operator function */

    /* Subtract equncy operator function.
     * ARGUMENTS:
     *   - adding Size:
     *       const size &Size;
     * RETURNS:
     *   (size &) - this Size referense.
     */
    size & operator-=( const size &Size )
    {
      *this = *this - Size;

      return *this;
    } /* End of '-=' operator function */

    /* Mutiply equncy operator function.
     * ARGUMENTS:
     *   - adding Size:
     *       const Type &Num;
     * RETURNS:
     *   (size &) - this Size referense.
     */
    size & operator*=( const Type &Num )
    {
      *this = *this * Num;

      return *this;
    } /* End of '*=' operator function */
    
    /* Zero Size function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (size &) - self Size referense.
     */
    size & Zero( void )
    {
      W = H = 0;

      return *this;
    } /* End of 'Zero' operator function */

    /* Returns distance between two vectors function.
     * ARGUMENTS:
     *   - second Size:
     *       const size &Size;
     * RETURNS:
     *   (FLT) - dsitance len.
     */
    FLT Distance( const size &Size ) const
    {
      return !(*this - Size);
    } /* End of 'Length2' operator function */

    /* '[]' operator function.
     * ARGUMENTS:
     *   - Size comp id:
     *       const INT &Id;
     * RETURNS:
     *   (Type &) - Size component.
     */
    Type & operator[]( const int &Id )
    {
      switch (Id)
      {
      case 0:
        return W;
      case 1:
        return H;
      default:
        return W;
      }
    } /* End of 'Length2' operator function */
  };
}
#endif // __mth_vec2_h_


/*
operator Type *
*/