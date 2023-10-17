/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mthdef.h
 * PURPOSE     : Raytracing project.
 *               Math default module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mthdef_h_
#define __mthdef_h_

typedef double DBL;
typedef double FLT;

template<typename Type> class vec2;
template<typename Type> class vec3;
template<typename Type> class vec4;

template<typename Type> class matr;
template<typename Type> class ray;
template<typename Type> class intr;
template<typename Type> class cam;
class noise;

/* defines */
#define PI 3.14159265358979323846

#define D2R(A) ((A) * (PI / 180))
#define R2D(A) ((A) / (PI / 180))


#endif // __mthdef_h_


