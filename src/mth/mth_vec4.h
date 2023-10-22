/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_vec4.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               !!! declaration file.
 * PROGRAMMER  : CGSG'2022.
 *               Maya Varennikova,
 * LAST UPDATE : 17.12.2022
 * NOTE        : Namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <iostream>

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include "mthdef.h"

/* Project namespace */
namespace mth
{
template<typename Type = FLT>
  class vec4
  {
    friend class vec3<Type>;
    friend class matr<Type>;


  public:

    Type X, Y, Z, W;    // Vector coordinates

    /* Default class constructor function.
     * ARGUMENTS: None.
     */
    vec4( VOID ): X(0), Y(0), Z(0), W(0)
    {
    } /* End of 'vec4' function. */

    /* Vector constructor function.
     * ARGUMENTS:
     *   - new values of vector parametrs:
     *       Type NewX, NewY, NewZ;
     */
    vec4( Type NewX, Type NewY, Type NewZ, Type NewW ): X(NewX), Y(NewY), Z(NewZ), W(NewW)
    {
    } /* End of 'vec4' function. */

    /* Vector constructor function.
     * ARGUMENTS:
     *   - new value of vector parametrs:
     *       Type A;
     */
    vec4( Type A ): X(A), Y(A), Z(A), W(A)
    {
    } /* End of 'vec4' function */

    /* Vector constructor function.
     * ARGUMENTS:
     *   - new value of vector parametrs (X, Y, Z):
     *       const vec3<Type> &V;
     *   - new value of vector parametrs (W):
     *       const Type N;
     */
    vec4( const vec3<Type> &V, const Type N = 0 ): X(V.X), Y(V.Y), Z(V.Z), W(N)
    {
    } /* End of 'vec4' function. */

    /* Vector constructor function.
     * ARGUMENTS:
     *   - new value of vector parametrs (X):
     *       const Type N;
     *   - new value of vector parametrs (Y, Z, W):
     *       const vec3<Type> &V;
     */
    vec4( const Type N, const vec3<Type> & V ) : X(N), Y(V.X), Z(V.Y), W(V.Z)
    {
    } /* End of 'vec4' function. */

    /* Get scalar product of vectors function.
     * ARGUMENTS:
     *   - source vector:
     *       const vec4 &V;
     * RETURNS:
     *   (FLT) result dot product.
     */
    FLT operator&( const vec4<Type> &V ) const
    {
      return X * V.X + Y * V.Y + Z * V.Z + W * V.W;
    } /* End of 'operator&' function. */

    /* Get squared lenght value of vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (FLT) squared lenght value.
     */
    FLT Lenght2( VOID ) const
    {
      return X * X + Y * Y + Z * Z + W * W;
    } /* End of 'lenght2' function */

    /* Get vector lenght function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (FLT) lenght of vector.
     */
    FLT operator!( VOID ) const
    {
      return sqrt(Lenght2());
    } /* End of 'operator!' function */

    /* Multiplicate vector with other vector function.
     * ARGUMENTS:
     *   - source vector:
     *       - const vec4 & V;
     * RETURNS:
     *   (vec4) result vector.
     */
    vec4 operator*( const vec4<Type> &V ) const
    {
      return vec4(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
    } /* End of 'operator*' function. */

    /* Add vectors function.
     * ARGUMENTS:
     *   - source vector:
     *       - const vec4 & V;
     * RETURNS:
     *   (vec4) result vector.
     */
    vec4 operator+( const vec4<Type> &V ) const
    {
      return vec4(X + V.X, Y + V.Y, Z + V.Z, W + V.W);
    } /* End of 'operator+' function. */

    /* Substact vectors function.
     * ARGUMENTS:
     *   - source vector:
     *       - const vec4 & V;
     * RETURNS:
     *   (vec4) result vector.
     */
    vec4 operator-( const vec4<Type> &V ) const
    {
      return vec4(X - V.X, Y - V.Y, Z - V.Z, W - V.W);
    } /* End of 'operator-' function */

    /* Get multiple with number vector function.
     * ARGUMENTS:
     *   - number to multiple:
     *       - const Type N;
     * RETURNS:
     *   (vec4) result vector.
     */
    vec4 operator*( const Type N ) const
    {
      return vec4(X * N, Y * N, Z * N, W * N);
    } /* End of 'operator*' function */

    /* Get substuct with number vector function.
     * ARGUMENTS:
     *   - number to substact:
     *       - const Type N;
     * RETURNS:
     *   (vec4) result vector.
     */
    vec4 operator/( const Type N ) const
    {
      if (N == 0)
        return *this;
      return vec4(X / N, Y / N, Z / N, W / N);
    } /* End of 'operator/' function */

    /* Add another vector function.
     * ARGUMENTS:
     *   - vector to add:
     *       - const vec4 &V;
     * RETURNS:
     *   (vec4 &) self reference.
     */
    vec4 & operator+=( const vec4 &V )
    {
      X += V.X, Y += V.Y, Z += V.Z, W += V.W;
      return *this;
    } /* End of 'operator+=' function */

    /* Substact another vector function.
     * ARGUMENTS:
     *   - vector to substruct:
     *       - const vec4 &V;
     * RETURNS:
     *   (vec4 &) self reference.
     */
    vec4 & operator-=( const vec4 &V )
    {
      X -= V.X, Y -= V.Y, Z -= V.Z, W -= V.W;
      return *this;
    } /* End of 'operator-=' function */

    /* Multiply vector with another vector function.
     * ARGUMENTS:
     *   - vector to multiple:
     *       - const vec4 &V;
     * RETURNS:
     *   (vec4 &) self reference.
     */
    vec4 & operator*=( const vec4 &V )
    {
      X *= V.X, Y *= V.Y, Z *= V.Z, W *= V.W;
      return *this;
    } /* End of 'operator*=' function */

    /* Devide vector with number function.
     * ARGUMENTS:
     *   - number to devide:
     *       - const Type N;
     * RETURNS:
     *   (vec4 &) self reference.
     */
    vec4 & operator/=( const Type N )
    {
      if (!N)
        return *this;
      X /= N, Y /= N, Z /= N, W /= N;
      return *this;
    } /* End of 'operator/=' function */

        /* Compere vector by components function.
     * ARGUMENTS:
     *   - vector to compere:
     *     const vec2 &V;
     * RETURNS:
     *   (BOOL) compering result.
     */
    BOOL operator>( const vec4 &V ) const
    {
      return X > V.X && Y > V.Y && Z > V.Z && W > V.W;
    } /* End of 'operator>' function. */

    /* Compere vector by components function.
     * ARGUMENTS:
     *   - vector to compere:
     *     const vec2 &V;
     * RETURNS:
     *   (BOOL) compering result.
     */
    BOOL operator<( const vec4 &V ) const
    {
      return X < V.X && Y < V.Y && Z < V.Z && W < V.W;
    } /* End of 'operator<' function. */

    /* Compere vector by components function.
     * ARGUMENTS:
     *   - vector to compere:
     *     const vec2 &V;
     * RETURNS:
     *   (BOOL) compering result.
     */
    BOOL operator==( const vec4 &V ) const
    {
      return X == V.X && Y == V.Y && Z == V.Z && W == V.W;
    } /* End of 'operator==' function. */

    /* Compere vector by components function.
     * ARGUMENTS:
     *   - vector to compere:
     *     const vec4 &V;
     * RETURNS:
     *   (BOOL) compering result.
     */
    BOOL operator>=( const vec4 &V ) const
    {
      return X >= V.X && Y >= V.Y && Z >= V.Z && W >= V.W;
    } /* End of 'operator>=' function. */

    /* Compere vector by components function.
     * ARGUMENTS:
     *   - vector to compere:
     *     const vec4 &V;
     * RETURNS:
     *   (BOOL) compering result.
     */
    BOOL operator<=( const vec4 &V ) const
    {
      return X <= V.X && Y <= V.Y && Z <= V.Z && W <= V.W;
    } /* End of 'operator<=' function. */

    /* Get normalized vector (self) function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec4 &) self reference.
     */
    vec4 & Normalize( VOID )
    {
      Type L = !*this;
      if (L != 0)
        X /= L, Y /= L, Z /= L, W /= L;
      return *this;
    } /* End of 'normalize' function */

    /* Get normalized vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec4) normalized vector.
     */
    vec4 Normalizing( VOID ) const
    {
      Type L = !*this;
      if (L != 0)
        return vec4(X / L, Y / L, Z / L, W / L);
      return *this;
    } /* End of 'normalizing' function */

    /* Get distance between vectors function.
     * ARGUMENTS:
     *   - destination vector:
     *       - const vec4 & V;
     * RETURNS:
     *   (FLT) distance value.
     */
    FLT Distance( const vec4 &V ) const
    {
      return !(V - *this);
    } /* End of 'distance' function */

    /* Find maximum vector component function.
     * ARGUMENTS: None.
     * RETURNS: 
     *   (Type) maximum component.
     */
    Type MaxC( VOID ) const
    {
      return max(max(max(X, Y), Z), W);
    } /* End of 'MaxC' function. */

    /* Find minimum vector component function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (Type) minimum component.
     */
    Type MinC( VOID ) const
    {
      return min(min(min(X, Y), Z), W);
    } /* End of 'MinC' function. */

    /* Get or set component of vector function.
     * ARGUMENTS:
     *   - component to get:
     *       - const UINT N;
     * RETURNS:
     *   (Type &) component value.
     */
    Type & operator[]( const UINT i )
    {
      switch (i)
      {
      case 3:
        return W;
      case 2:
        return Z;
      case 1:
        return Y;
      case 0:
        return X;
      default:
        throw std::runtime_error("Invalid index");
      }
    } /* End of 'operator[]' function */

  }; /* End of 'vec4' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */
