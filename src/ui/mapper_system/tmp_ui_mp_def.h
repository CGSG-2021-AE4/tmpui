/***************************************************************
 * Copyright (C) 2021-2022
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tmp_ui_mp_def.h
 * PURPOSE     : Tough Megapolis Planner project.
 *               general includeing file for mapper system.
 * PROGRAMMER  : CGSG'2022.
 *               Ilias Safin,
 * LAST UPDATE : 22.12.2022
 * NOTE        : Namespace 'ui', 'mapper'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

// #include "tmp_def.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <Windows.h>

#define CHECK_TYPE_CONFORMITY(Type) std::is_same_v<Type, INT> || std::is_same_v<Type, FLOAT> || std::is_same_v<Type, BOOL> || std::is_same_v<Type, vec2> || std::is_same_v<Type, vec3> || std::is_same_v<Type, vec4>

/* END OF 'tmp_ui_mp_def.h' FILE */
