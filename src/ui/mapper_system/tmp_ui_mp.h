/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tmp_ui_mp.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               general file of mapper system for including.
 * PROGRAMMER  : CGSG'2022.
 *               Ilias Safin,
 * LAST UPDATE : 22.12.2022
 * NOTE        : Namespace 'ui', 'mapper'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tmp_ui_mp_h_
#define __tmp_ui_mp_h_

#include "tmp_ui_mp_value.h"

namespace tmp
{
  namespace ui
  {
    namespace mapper
    {
      typedef mp_value<INT> mp_int;
      typedef mp_value<BOOL> mp_bool;
      typedef mp_value<FLOAT> mp_flt;
      typedef mp_value<vec2> mp_vec2;
      typedef mp_value<vec3> mp_vec3;
      typedef mp_value<vec4> mp_vec4;
    } /* End of 'mapper' namespace */
  } /* End of 'ui' namespace */
} /* End of 'tmp' namespace */
#endif /* __tmp_ui_mp_h_ */

/* END OF 'tmp_ui_mp.h' FILE */
