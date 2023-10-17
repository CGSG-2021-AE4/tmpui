/*************************************************************
 * Copyright (C) 2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_ray.h
 * PURPOSE     : Raytracing project.
 *               Math ray functions module.
 * PROGRAMMER  : CGSG-SummerCamp'2022.
 *               Andrey Egorov.
 * LAST UPDATE : 24.07.2022.
 * NOTE        : Module namespace 'tmp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


#ifndef __mth_ray_h_
#define __mth_ray_h_
#include "mthdef.h"

namespace mth
{
  template<typename Type>
  class ray
  {
  public:
    vec3<Type> Org, Dir; // Ray begin location and direction 

    /* Default ray construct function */
    ray()
    {
    }

    /* Ray construct function */
    ray( vec3<Type> NewOrg, vec3<Type> NewDir ) : Org(NewOrg), Dir(NewDir)
    {
    }

    vec3<Type> operator()( DBL &t )
    {
      return Org + Dir * t;
    }
  };

  
}


#endif // __mth_ray_h_
