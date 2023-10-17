/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : def.h
 * PURPOSE     : Raytracing project.
 *               Main handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 23.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __def_h_
#define __def_h_

#include <windows.h>

#undef max
#undef min


/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __mem_debug 
{ 
public: 
  /* Class constructor */
  __mem_debug( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __mem_alloc_debug;


# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

#include "./mth/mth.h"

namespace tmp
{
  typedef mth::vec4<FLT> vec4;
  typedef mth::vec3<FLT> vec3;
  typedef mth::vec2<FLT> vec2;

  typedef mth::vec3<INT> ivec3;
  typedef mth::vec2<INT> ivec2;

  typedef mth::size<INT> size;

  typedef mth::ray<DBL> ray;
  typedef mth::cam<DBL> cam;
}


#include "./win/win.h"
#include ".\frame\frame.h"
//#include "./rt/shapes/render.h"

#endif // __def_h_

/* END OF 'def.h' FILE*/